#include <stdio.h>
#include <stdint.h>

#define BUFFER_SIZE 8U

typedef struct
{
    uint8_t data[BUFFER_SIZE];
    uint8_t head;
    uint8_t tail;
    uint8_t count;
} RingBuffer;

void ringbuf_init(RingBuffer *rb)
{
    rb->head = 0U;
    rb->tail = 0U;
    rb->count = 0U;
}

int ringbuf_write(RingBuffer *rb, uint8_t value)
{
    if (rb->count == BUFFER_SIZE)
        return -1;

    rb->data[rb->head] = value;
    rb->head = (uint8_t)((rb->head + 1U) & (BUFFER_SIZE - 1U));
    rb->count++;

    return 0;
}

int ringbuf_read(RingBuffer *rb, uint8_t *value)
{
    if (rb->count == 0U)
        return -1;

    *value = rb->data[rb->tail];
    rb->tail = (uint8_t)((rb->tail + 1U) & (BUFFER_SIZE - 1U));
    rb->count--;

    return 0;
}

uint8_t ringbuf_count(const RingBuffer *rb)
{
    return rb->count;
}

int ringbuf_is_full(const RingBuffer *rb)
{
    return rb->count == BUFFER_SIZE;
}

int ringbuf_is_empty(const RingBuffer *rb)
{
    return rb->count == 0U;
}

int main(void)
{
    RingBuffer rb;
    uint8_t value;
    uint8_t i;

    const uint8_t initial_data[] = {
        0x41U, 0x42U, 0x43U, 0x44U,
        0x45U, 0x46U, 0x47U, 0x48U
    };

    const uint8_t new_data[] = {
        0x49U, 0x4AU, 0x4BU
    };

    ringbuf_init(&rb);

    printf("=== Ring Buffer Demonstration ===\n\n");

    printf("Step 1: Fill buffer\n");

    for (i = 0U; i < BUFFER_SIZE; i++)
    {
        if (ringbuf_write(&rb, initial_data[i]) == 0)
        {
            printf("[WRITE] 0x%02X -> OK (count=%u)",
                   (unsigned int)initial_data[i],
                   (unsigned int)ringbuf_count(&rb));

            if (ringbuf_is_full(&rb))
                printf(" FULL");

            printf("\n");
        }
    }

    printf("Buffer full: %s, count=%u\n\n",
           ringbuf_is_full(&rb) ? "YES" : "NO",
           (unsigned int)ringbuf_count(&rb));

    printf("Step 2: Write when full\n");

    if (ringbuf_write(&rb, 0x99U) != 0)
        printf("[WRITE] 0x99 -> FAIL (buffer full)\n\n");

    printf("Step 3: Read three bytes\n");

    for (i = 0U; i < 3U; i++)
    {
        if (ringbuf_read(&rb, &value) == 0)
        {
            printf("[READ] -> 0x%02X (count=%u)\n",
                   (unsigned int)value,
                   (unsigned int)ringbuf_count(&rb));
        }
    }

    printf("\nStep 4: Write three new bytes\n");

    for (i = 0U; i < 3U; i++)
    {
        if (ringbuf_write(&rb, new_data[i]) == 0)
        {
            printf("[WRITE] 0x%02X -> OK (count=%u)",
                   (unsigned int)new_data[i],
                   (unsigned int)ringbuf_count(&rb));

            if (ringbuf_is_full(&rb))
                printf(" FULL");

            printf("\n");
        }
    }

    printf("\nStep 5: Read remaining bytes\n");

    while (!ringbuf_is_empty(&rb))
    {
        if (ringbuf_read(&rb, &value) == 0)
        {
            printf("[READ] -> 0x%02X (count=%u)\n",
                   (unsigned int)value,
                   (unsigned int)ringbuf_count(&rb));
        }
    }

    printf("Buffer empty: %s\n\n",
           ringbuf_is_empty(&rb) ? "YES" : "NO");

    printf("Step 6: Read when empty\n");

    if (ringbuf_read(&rb, &value) != 0)
        printf("[READ] (empty) -> FAIL (buffer empty)\n");

    return 0;
}
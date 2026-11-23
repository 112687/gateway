#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#include "app_bt.h"

enum { INPUT_CAPACITY = 128 };

static size_t make_frame(uint8_t *frame, const char *payload)
{
    size_t payload_len = strlen(payload);
    assert(payload_len >= 1 && payload_len <= 12);

    frame[0] = 0xF1;
    frame[1] = 0xDD;
    frame[2] = (uint8_t)(4 + payload_len);
    frame[3] = 0x00;
    frame[4] = 0x88;
    frame[5] = 0xFF;
    frame[6] = 0xFF;
    memcpy(frame + 7, payload, payload_len);
    return 7 + payload_len;
}

static void assert_message(const char *output, int output_len,
                           const char *expected_payload)
{
    size_t payload_len = strlen(expected_payload);

    assert(output_len == (int)(5 + payload_len));
    assert((uint8_t)output[0] == 1);
    assert((uint8_t)output[1] == 2);
    assert((uint8_t)output[2] == payload_len);
    assert((uint8_t)output[3] == 0x00);
    assert((uint8_t)output[4] == 0x88);
    assert(memcmp(output + 5, expected_payload, payload_len) == 0);
}

static int feed(const uint8_t *input, size_t input_len, char *output)
{
    assert(input_len <= INPUT_CAPACITY);
    memset(output, 0, INPUT_CAPACITY);
    memcpy(output, input, input_len);
    return app_bt_postRead(output, (int)input_len);
}

static void assert_frame(const char *payload)
{
    uint8_t frame[INPUT_CAPACITY];
    char output[INPUT_CAPACITY];
    size_t frame_len = make_frame(frame, payload);
    int output_len = feed(frame, frame_len, output);

    assert_message(output, output_len, payload);
}

int main(void)
{
    uint8_t frame[INPUT_CAPACITY];
    char output[INPUT_CAPACITY];
    size_t frame_len;

    /* A later complete frame must not replay the previous payload. */
    assert_frame("25");
    assert_frame("70");

    /* A frame split across reads is buffered until its final byte arrives. */
    frame_len = make_frame(frame, "70");
    assert(feed(frame, 4, output) == 0);
    assert(feed(frame + 4, frame_len - 4, output) == 7);
    assert_message(output, 7, "70");

    /* AT response noise before a valid frame must be skipped. */
    {
        uint8_t stream[INPUT_CAPACITY];
        size_t valid_len = make_frame(frame, "25");
        memcpy(stream, "OK\r\n", 4);
        memcpy(stream + 4, frame, valid_len);
        assert(feed(stream, 4 + valid_len, output) == 7);
        assert_message(output, 7, "25");
    }

    /* An invalid high-bit length must not block the following valid frame. */
    {
        uint8_t stream[INPUT_CAPACITY] = { 0xF1, 0xDD, 0x80 };
        size_t valid_len = make_frame(frame, "70");
        memcpy(stream + 3, frame, valid_len);
        assert(feed(stream, 3 + valid_len, output) == 7);
        assert_message(output, 7, "70");
    }

    puts("app_bt_postRead regression tests passed");
    return 0;
}


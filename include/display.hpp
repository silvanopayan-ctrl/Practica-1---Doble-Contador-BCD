#ifndef DISPLAY_HPP
#define DISPLAY_HPP

#include <cstdint>

namespace practica
{

class SevenSegmentDisplay
{
public:
    void init();
    void showDigit(uint8_t display_id, uint8_t digit);
    void blank(uint8_t display_id);
    void blankAll();

private:
    void writeSegments(uint8_t pattern);
};

void display_refresh_task(void *pvParameters);

} // namespace practica

#endif
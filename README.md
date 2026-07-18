# STM32F205-LCD

Bare-metal HD44780-compatible 8-bit parallel LCD driver for **STM32F205RCT6**.
Direct register access — no HAL dependency beyond `stm32f2xx.h` for CMSIS definitions.

> Likely portable to other STM32F2 parts; only the GPIO clock-enable bits in `enable_gpio_clock()` are family-specific.

## Wiring

Define the following pins in CubeMX (or by hand) as GPIO outputs and label them:

| Function        | Label          |
|-----------------|----------------|
| Data 0–7        | `D0` .. `D7`   |
| Enable          | `E`            |
| Read/Write      | `RW`           |
| Register Select | `RS`           |

The library reads `D0_Pin` / `D0_GPIO_Port` etc. directly, so the **labels are what matter**.

## Usage

```c
#include "LCDLib.h"

int main(void)
{
    // ... clock / HAL init ...

    LCD_Init();
    LCD_WriteString("Hello, STM32F205");
    LCD_SetCursor(0, 1);
    LCD_WriteInt(42);

    while (1) { }
}
```

## API

| Function                  | Purpose                                                   |
|---------------------------|-----------------------------------------------------------|
| `LCD_Init()`              | Configure GPIO and run HD44780 init sequence              |
| `LCD_Clear()`             | Clear display, return cursor home                         |
| `LCD_SetCursor(x, y)`     | Move cursor to column `x`, row `y`                        |
| `LCD_WriteChar(c)`        | Print single character                                    |
| `LCD_WriteString(s)`      | Print null-terminated string                              |
| `LCD_WriteInt(v)`         | Print integer                                             |
| `LCD_WriteFloat(v)`       | Print float (requires `printf` float support enabled)     |
| `LCD_SendCommand(cmd)`    | Raw HD44780 command escape hatch                          |

## Notes

- Busy-wait delays are tuned for default STM32F205 clocks — adjust `LCD_DELAY_E` / `LCD_DELAY_D` in `LCDLib.h` if you change `SYSCLK`.
- `LCD_WriteFloat` pulls in `printf` float formatting; if you're tight on flash, drop it.
- The driver is write-only; the `R/W` pin is held low. If you need busy-flag polling, extend `send_byte()`.

## License

MIT — see [LICENSE](LICENSE).

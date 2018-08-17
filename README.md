# Nintendo Switch Pro Controller AVR Firmware (Not Functional)

It seems that a recent Controller Firmware Update has changed the endpoint types from URB_INTERRUPT to URB_BULK, thus this firmware is no longer compatible with recent versions of the Switch firmware. I do not have the required hardware to attempt to figure out the new format, so i'm unable to fix this. If I ever get the proper hardware i'll be more than happy to fix it, but for now, consider this non-working.

Back-up of old text below.

>Firmware for AVR devices to emulate a genuine Nintendo Switch Pro Controller by AterialDawn
>
>Project Layout is for Atmel Studio 7.0, so certain things (#pragma region for example) might give warnings/errors when used anywhere else.
>
>Licensed under GPLv3

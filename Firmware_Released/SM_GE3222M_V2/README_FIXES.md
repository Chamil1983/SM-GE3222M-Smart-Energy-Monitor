SM_GE3222M V2.0.1 - v6.3 Modbus RTU Abort Fix (No Callbacks)

What was happening
- ESP32 was resetting with "abort() was called ..." right after:
  "ModbusServer: Register map added; registering callbacks..."

Root cause
- In some ESP32 builds of the emelianov/modbus-esp8266 library, registering large-range callbacks
  (onGet*/onSet*) can hit an internal assert/abort. This is separate from heap/WDT.

What changed (safe permanent fix)
1) Modbus RTU now runs WITHOUT callbacks.
   - We still create the register map (Ireg/Hreg/Coil/Ists).
   - Meter/System updates WRITE DIRECTLY into the library register store via:
       Ireg(addr, value), Hreg(addr, value), Coil(addr, state), Ists(addr, state)
   - Reads likewise use Ireg(addr), Hreg(addr), Coil(addr), Ists(addr).

2) FEATURE_MODBUS_TCP is set to 0 in Version.h (RTU-only).

Files changed
- ModbusServer.cpp: callbacks removed + direct register store sync added
- Version.h: FW_VERSION=2.0.1, FEATURE_MODBUS_TCP=0

If you still see resets
- Paste the FULL backtrace after abort(), and the decoded source line (addr2line) so we can target it.

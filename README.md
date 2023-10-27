```
███████╗ ██████╗ ██████╗  ██████╗███████╗
██╔════╝██╔═══██╗██╔══██╗██╔════╝██╔════╝
█████╗  ██║   ██║██████╔╝██║     █████╗  
██╔══╝  ██║   ██║██╔══██╗██║     ██╔══╝  
██║     ╚██████╔╝██║  ██║╚██████╗███████╗
╚═╝      ╚═════╝ ╚═╝  ╚═╝ ╚═════╝╚══════╝
                                         
██████╗ ████████╗███████╗                
██╔══██╗╚══██╔══╝██╔════╝                
██████╔╝   ██║   ███████╗                
██╔══██╗   ██║   ╚════██║                
██║  ██║   ██║   ███████║                
╚═╝  ╚═╝   ╚═╝   ╚══════╝                
```

An `LD_PRELOAD`-able library to set RTS low when `open()`-ing a serial port.

## Why?

In both the microcontroller and HAM world, it is common to repurpose modem control signals (e.g., to reset a microcontroller, or to control the PTT switch on a radio). Under Linux, opening a serial port will automatically set the RTS signal, which can have unintended and annoying consequences.

This library works by clearing RTS as soon as the port is opened.

See also:

- https://stackoverflow.com/q/5090451/147356
- https://github.com/picocom-ng/picocom/blob/main/lowerrts.md

## How?

Compile the library:

```
$ make
```

Set the `FORCE_RTS_DEVICES` environment variable to a colon-delimited list of serial devices (or glob patterns) for which we should override the RTS value:

```
export FORCE_RTS_DEVICES="/dev/ttyS0:/dev/ttyUSB*"
```

And then `LD_PRELOAD` the library when running software that access the serial port:

```
$ export LD_PRELOAD=$PWD/force_rts.so
$ some comand ...
```

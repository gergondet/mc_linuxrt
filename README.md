MC Linux RT
==

This package contains various executables to work with mc\_rtc with a Linux PREEMPT RT kernel

Available executable
--

- `MCLinuxRTMeasureLatency`: measure RT latency for an empty loop
- `MCLinuxRT`: runs mc\_rtc inside the RT loop
- `MCLinuxRTServer`: runs an UDP server inside the RT loop, connection can be established using `MCUDPControl` from the [mc\_udp package](https://github.com/jrl-umi3218/mc_udp)

Environment variables
--

By setting the `MC_RTC_FREQ` environment variable, one can set the desired loop frequency (in millisecond)

Examples
--

Run the latency measurer with a 1ms loop (default):

```bash
sudo MCLinuxRTMeasureLatency
```

Run the latency measurer with a 5ms loop:

```bash
sudo MC_RTC_FREQ=5 MCLinuxRTMeasureLatency
```

Run mc\_rtc in Linux RT with a 5ms loop (frequency is deduced from mc\_rtc configuration):

```bash
sudo MCLinuxRT $HOME/.config/mc_rtc/mc_rtc.conf
```

Run the UDP server on port 4444 (default):

```bash
sudo MC_RTC_FREQ=5 MCLinuxRTServer 4444
```

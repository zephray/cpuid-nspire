# cpuid-nspire

A simple tool to read and show CPU and cache information for TI-Nspire series calculators. Requires Ndless to run.

You will probably not gonna to find anything interesting, all TI-nspire calculators till now are using the ARM926EJ-s core with 16KB I-cache and 8KB D-cache. Only difference is that some are using r4p0 version of the core while some newer ones are using r5p0 version of the core.

I was wondering if CX-II changed anything or even switched to FA626TE core given that they are using lots of Faradays' IP core in CX-II SoC, so I wrote this tool. And, nope. It is still 926EJs by ARM.

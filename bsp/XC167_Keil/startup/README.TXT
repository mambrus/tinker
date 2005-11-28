IMPRORANT: Start_v2.A66 CONTAINS A PATCH FOR THE CONTROL REGISTER OF THE EXTERNAL BUS

Patchs for:

1. External memory RAM bus
2. Non resetting of RAM in case of warm-boot (We want TinKer ti be able to detect and handle that case).



To keep projects generic, all dave project must me called Dave.dav, so that the produce a .asm file with a pre-known name (i.e. Dave.asm) since this file will be read by Start_V2.A66. This dependancy and pre-requisity is part of the above patch and might dissapere later.
(Start_v2.A66 includes a ASM file produced by Dave and it's doing this by name)

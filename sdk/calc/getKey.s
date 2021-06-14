.align 2
.global _getKey
.type _getKey, @function

_getKey:
	mov.l GetKey_keyAddr, r2 !Load the keyboard addr
	mov.l GetKey_and, r3
	mov.w @r2, r1   !Load first word in r1
	shll16 r1
	add #2, r2
	mov.w @r2, r0   !Load 2nd word in r0
	and r3, r0  !Just use the lower 16 bits
	add r0, r1  !Add both in r1
	mov.l r1, @r4   !Store in first argument
	add #2, r2
	mov.w @r2, r1   !Load 3rd word in r1
	shll16 r1
	add #2, r2
	mov.w @r2, r0   !Load 4th word in r0
	and r3, r0  !Just use the lower 16 bits
	add r0, r1  !Add both in r1
	mov.l r1, @r5   !Store in 2nd argument
	add #2, r2
	mov.w @r2, r1   !Load 3rd because otherwise the hardware doesn't work
	add #2, r2
	rts
	mov.w @r2, r1   !Load 4th because of the hardware.

.align 2
GetKey_keyAddr:
	.long 0xa44B0000
GetKey_and:
	.long 0x0000FFFF


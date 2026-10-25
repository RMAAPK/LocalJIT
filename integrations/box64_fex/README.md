# Local-JIT / Box64 & FEX-Emu Translation Cache Specification

## Context
Box64 and FEX-Emu dynamically JIT-translate x86/x86_64 machine instructions into ARM64 instructions for mobile, handheld consoles (Steam Deck ARM mods, Ayn Odin, Raspberry Pi 5), and server Graviton platforms.

## The Bottleneck: Dynamic Block Translation & Invalidation
- High-performance engines frequently generate dynamic code blocks (JIT-in-JIT, script engines, Mono, .NET, Unreal Blueprint VM).
- Translating basic blocks repeatedly incurs translation cache thrashing.

## Local-JIT P2P Block Cache
Local-JIT bridges Box64/FEX translation pages with a content-addressed storage layer:

1. **Deterministic Basic Block Hashing**:
   - Takes input x86 instruction sequences $\to$ canonical hash $\to$ yields pre-translated, verified ARM64 machine code (`mul`, `add`, `ldr`, `str`).
2. **Sub-Microsecond Dispatch**:
   - Rather than invoking the software dynarec to walk bytecode trees, Box64 loads bit-exact native ARM64 blocks directly from Local-JIT RAM.
3. **Verified Zero-Knowledge Safety**:
   - Blocks are validated bit-exact using execution test vectors, preserving hardware exception correctness across architectures.

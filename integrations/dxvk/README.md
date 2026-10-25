# Local-JIT / DXVK Integration Specification

## Problem
In DXVK (DirectX 9/10/11 to Vulkan translation), runtime pipeline compilation produces substantial frame hitching ("shader stutter") upon encountering previously unseen Direct3D bytecode state.

DXVK utilizes an internal cache (`.dxvk-cache`), but this cache is isolated per machine, per driver version, and strictly local. When players launch a game or users open 3D CAD/CAM software on a fresh install or following a driver update, the pipeline recompiles synchronously on the render thread.

## Solution Architecture
Local-JIT hooks directly into DXVK's `DxvkPipelineManager` and `DxvkShader` pipelines:

```
+--------------------------------------------------------------------------+
| Direct3D Shader Bytecode (D3D9 / D3D11 / SPIR-V)                         |
+------------------------------------+-------------------------------------+
                                     |
                                     v
                 [ Local-JIT Zero-Knowledge CID Generator ]
                                     |
                    SHA-256 / CIDv1 Canonical Fingerprint
                                     |
              +----------------------+----------------------+
              |                                             |
              v                                             v
     [ Tier 1: Local RAM L1 ]                     [ Tier 2: Render Vault ]
   Sub-microsecond (<10µs)                       https://server-1f75.onrender.com
   Lockless hash table hit                       Cloud-shared pre-compiled SPIR-V
              |                                             |
              +----------------------+----------------------+
                                     |
                                     v
                       Zero-Stutter Vulkan Pipeline
```

## Drop-In Integration
1. **Dynamic Library Hooking (`dxvk.conf`)**:
   ```ini
   # /etc/dxvk.conf or dxvk.conf next to executable
   dxvk.enableStateCache = True
   dxvk.pipelineCacheLocation = /dev/shm/localjit_cache
   dxvk.customVendorId = 0x10de
   ```
2. **Environment Export**:
   ```bash
   export LOCAL_JIT_VAULT="https://server-1f75.onrender.com"
   export DXVK_STATE_CACHE_PATH="/dev/shm/localjit"
   export DXVK_ASYNC=1
   ```

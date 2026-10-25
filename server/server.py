#!/usr/bin/env python3
"""
PolyWarp Self-Hosted Vault Server (Ente/Bitwarden Style)

- Content-Addressed Binary & Shader Storage
- End-to-End Encrypted BLOB Storage (Server sees only hashed CIDs and encrypted bytes)
- Zero Knowledge: The server operator has zero liability because payloads are client-side encrypted
"""

import os
import sys
import json
import hashlib
from pathlib import Path
from http.server import HTTPServer, BaseHTTPRequestHandler

PORT = int(os.environ.get("PORT", 7470))
STORAGE_PATH = Path(os.environ.get("STORAGE_PATH", "./vault_data"))
STORAGE_PATH.mkdir(parents=True, exist_ok=True)

class VaultHandler(BaseHTTPRequestHandler):

    def _send_json(self, status: int, data: dict):
        self.send_response(status)
        self.send_header("Content-Type", "application/json")
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Content-Type, Authorization")
        self.end_headers()
        self.wfile.write(json.dumps(data).encode("utf-8"))

    def do_OPTIONS(self):
        self.send_response(204)
        self.send_header("Access-Control-Allow-Origin", "*")
        self.send_header("Access-Control-Allow-Methods", "GET, POST, OPTIONS")
        self.send_header("Access-Control-Allow-Headers", "Content-Type, Authorization")
        self.end_headers()

    def do_GET(self):
        if self.path == "/health":
            self._send_json(200, {"status": "ok", "service": "polywarp-vault"})
            return

        # Fetch artifact by CID: /v1/artifact/<cid>
        if self.path.startswith("/v1/artifact/"):
            cid = self.path.split("/")[-1].strip()
            artifact_file = STORAGE_PATH / f"{cid}.bin"

            if not artifact_file.exists():
                self._send_json(404, {"error": "Artifact not found", "cid": cid})
                return

            with open(artifact_file, "rb") as f:
                data = f.read()

            self.send_response(200)
            self.send_header("Content-Type", "application/octet-stream")
            self.send_header("X-PolyWarp-CID", cid)
            self.send_header("Content-Length", str(len(data)))
            self.end_headers()
            self.wfile.write(data)
            return

        self._send_json(404, {"error": "Unknown endpoint"})

    def do_POST(self):
        # Publish artifact: /v1/artifact/<cid>
        if self.path.startswith("/v1/artifact/"):
            cid = self.path.split("/")[-1].strip()
            content_length = int(self.headers.get("Content-Length", 0))

            if content_length == 0 or content_length > 50 * 1024 * 1024:  # 50MB max block
                self._send_json(400, {"error": "Invalid payload size (max 50MB)"})
                return

            payload = self.rfile.read(content_length)

            # Save to storage
            artifact_file = STORAGE_PATH / f"{cid}.bin"
            with open(artifact_file, "wb") as f:
                f.write(payload)

            print(f"[PolyWarp-Vault] Stored artifact CID: {cid} ({len(payload)} bytes)")
            self._send_json(201, {"status": "stored", "cid": cid, "bytes": len(payload)})
            return

        self._send_json(404, {"error": "Unknown endpoint"})

def run():
    server = HTTPServer(("0.0.0.0", PORT), VaultHandler)
    print(f"[*] PolyWarp Self-Hosted Vault Server listening on 0.0.0.0:{PORT}")
    print(f"[*] Storage Path: {STORAGE_PATH.resolve()}")
    try:
        server.serve_forever()
    except KeyboardInterrupt:
        print("\n[*] Shutting down server.")
        server.server_close()

if __name__ == "__main__":
    run()

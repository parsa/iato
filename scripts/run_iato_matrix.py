#!/usr/bin/env python3
"""Run IATO clients across the IA64 binaries listed in misc/."""

from __future__ import annotations

import argparse
import json
import os
import subprocess
import sys
import time
from pathlib import Path
from typing import Dict, List


REPO_ROOT = Path(__file__).resolve().parents[1]
DEFAULT_MANIFEST = REPO_ROOT / "build/manifest/misc_ia64_binaries.json"
DEFAULT_LOG_ROOT = REPO_ROOT / "build/logs"
DEFAULT_TRACE_ROOT = REPO_ROOT / "build/traces"

CLIENT_CONFIG: Dict[str, Dict] = {
    "iaos": {
        "binary": REPO_ROOT / "build/bin/iaos",
        "flags": ["-e"],
        "category_flags": {},
        "timeout": 60,
        "input_kind": "binary",
    },
    "iaka": {
        "binary": REPO_ROOT / "build/bin/iaka",
        "flags": ["-s", "-m", "10000"],
        "category_flags": {
            "spec": ["-s", "-m", "4000"],
        },
        "timeout": 120,
        "trace_template": ["-t:f", "{trace}"],
        "input_kind": "binary",
    },
    "iaio": {
        "binary": REPO_ROOT / "build/bin/iaio",
        "flags": ["-s", "-m:i", "20000", "-m:c", "20000"],
        "category_flags": {
            "spec": ["-s", "-m:i", "6000", "-m:c", "6000"],
        },
        "timeout": 180,
        "input_kind": "binary",
    },
    "iaoo": {
        "binary": REPO_ROOT / "build/bin/iaoo",
        "flags": ["-s", "-m:i", "20000", "-m:c", "20000"],
        "category_flags": {
            "spec": ["-s", "-m:i", "6000", "-m:c", "6000"],
        },
        "timeout": 180,
        "input_kind": "binary",
    },
    "iata": {
        "binary": REPO_ROOT / "build/bin/iata",
        "flags": ["-s"],
        "category_flags": {},
        "timeout": 60,
        "input_kind": "trace",
    },
}


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "--clients",
        default="iaos,iaka,iaio,iaoo,iata",
        help="Comma-separated list of clients to run (default: all)",
    )
    parser.add_argument(
        "--manifest",
        default=str(DEFAULT_MANIFEST),
        help="Path to IA64 binary manifest JSON",
    )
    parser.add_argument(
        "--log-root",
        default=str(DEFAULT_LOG_ROOT),
        help="Where to store per-client logs",
    )
    parser.add_argument(
        "--trace-root",
        default=str(DEFAULT_TRACE_ROOT),
        help="Where to store generated traces",
    )
    parser.add_argument(
        "--categories",
        default="",
        help="Optional comma list of categories (e.g., clt,spec) to include",
    )
    parser.add_argument(
        "--limit",
        type=int,
        default=0,
        help="Maximum number of binaries to process (0 = no limit)",
    )
    parser.add_argument(
        "--emit-traces",
        action="store_true",
        help="When set, emit emulator traces for clients that support it",
    )
    parser.add_argument(
        "--iata-trace-source",
        default="iaka",
        help="Client name whose traces feed IATA (default: iaka)",
    )
    return parser.parse_args()


def load_manifest(path: Path) -> List[Dict]:
    with path.open() as fh:
        data = json.load(fh)
    for entry in data:
        entry.setdefault("category", derive_category(entry["path"]))
    return data


def derive_category(path: str) -> str:
    parts = Path(path).parts
    return parts[1] if len(parts) > 1 else "unknown"


def slugify(path: str) -> str:
    return path.replace("/", "__")


def ensure_binaries(clients: List[str]) -> None:
    missing = [c for c in clients if not CLIENT_CONFIG[c]["binary"].exists()]
    if missing:
        raise FileNotFoundError(
            f"Missing client binaries: {', '.join(missing)}. "
            "Run the CMake build first."
        )


def main() -> int:
    args = parse_args()
    clients = [c.strip() for c in args.clients.split(",") if c.strip()]
    manifest_path = Path(args.manifest)
    log_root = Path(args.log_root)
    trace_root = Path(args.trace_root)
    categories_filter = {c.strip() for c in args.categories.split(",") if c.strip()}

    ensure_binaries(clients)

    entries = load_manifest(manifest_path)
    if categories_filter:
        entries = [e for e in entries if e.get("category") in categories_filter]
    if args.limit and args.limit > 0:
        entries = entries[: args.limit]

    if not entries:
        print("No binaries selected; nothing to do.")
        return 0

    log_root.mkdir(parents=True, exist_ok=True)
    trace_root.mkdir(parents=True, exist_ok=True)

    env = os.environ.copy()
    ld_paths = [
        str(REPO_ROOT / "third_party/libelf/usr/lib/x86_64-linux-gnu"),
        env.get("LD_LIBRARY_PATH", ""),
    ]
    env["LD_LIBRARY_PATH"] = ":".join([p for p in ld_paths if p])

    for client in clients:
        cfg = CLIENT_CONFIG[client]
        client_dir = log_root / client
        client_dir.mkdir(parents=True, exist_ok=True)
        client_trace_dir = trace_root / client
        client_trace_dir.mkdir(parents=True, exist_ok=True)
        summary = []
        for entry in entries:
            start = time.time()
            slug = slugify(entry["path"])
            log_path = client_dir / f"{slug}.log"
            category = entry.get("category") or derive_category(entry["path"])

            flags = list(cfg["category_flags"].get(category, cfg["flags"]))
            trace_path = None
            if args.emit_traces and cfg.get("trace_template"):
                trace_path = client_trace_dir / f"{slug}.trc"
                template = cfg["trace_template"]
                if isinstance(template, str):
                    rendered = [template.format(trace=trace_path)]
                else:
                    rendered = [part.format(trace=trace_path) for part in template]
                flags = list(flags) + rendered
                trace_path.parent.mkdir(parents=True, exist_ok=True)

            input_path = entry["path"]
            if cfg.get("input_kind") == "trace":
                trace_source = trace_root / args.iata_trace_source
                trace_source_path = trace_source / f"{slugify(entry['path'])}.trc"
                if not trace_source_path.exists():
                    with log_path.open("w") as log_fh:
                        log_fh.write(
                            f"Trace missing: {trace_source_path}. "
                            "Run a trace-producing client first.\n"
                        )
                    summary.append(
                        {
                            "binary": entry["path"],
                            "category": category,
                            "client": client,
                            "exit_code": None,
                            "timed_out": False,
                            "duration_sec": 0,
                            "log": str(log_path.relative_to(REPO_ROOT)),
                            "trace": None,
                            "skipped": "missing-trace",
                        }
                    )
                    print(f"[{client}] skipped {entry['path']} (missing trace)")
                    continue
                trace_path = trace_source_path
                input_path = str(trace_source_path)

            cmd = [str(cfg["binary"])] + flags + [input_path]
            timeout = cfg.get("timeout")
            exit_code = None
            timed_out = False
            try:
                with log_path.open("w") as log_fh:
                    log_fh.write(f"$ {' '.join(cmd)}\n\n")
                    result = subprocess.run(
                        cmd,
                        stdout=log_fh,
                        stderr=subprocess.STDOUT,
                        timeout=timeout,
                        env=env,
                        cwd=REPO_ROOT,
                        check=False,
                    )
                    exit_code = result.returncode
            except subprocess.TimeoutExpired:
                timed_out = True
                with log_path.open("a") as log_fh:
                    log_fh.write(f"\n*** TIMEOUT after {timeout} seconds ***\n")

            duration = time.time() - start
            summary.append(
                {
                    "binary": entry["path"],
                    "category": category,
                    "client": client,
                    "exit_code": exit_code,
                    "timed_out": timed_out,
                    "duration_sec": round(duration, 3),
                    "log": str(log_path.relative_to(REPO_ROOT)),
                    "trace": str(trace_path.relative_to(REPO_ROOT))
                    if trace_path
                    else None,
                }
            )
            status = "TIMEOUT" if timed_out else exit_code
            print(f"[{client}] {entry['path']} -> {status} ({duration:.1f}s)")

        summary_path = client_dir / "summary.json"
        with summary_path.open("w") as fh:
            json.dump(summary, fh, indent=2)
        print(f"Wrote {summary_path}")

    return 0


if __name__ == "__main__":
    sys.exit(main())


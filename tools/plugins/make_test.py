from __future__ import annotations

import subprocess
from pathlib import Path
from typing import Optional

from pydantic import Field

from checker.plugins import PluginABC, PluginOutput
from checker.exceptions import PluginExecutionFailed
from checker.utils import print_info


class MakeTestPlugin(PluginABC):
    """Plugin that runs `make test` for a task."""
    
    name = "make_test"

    class Args(PluginABC.Args):
        task_dir: str
        timeout: int = 180
        build_cmd: list[str] = Field(default_factory=lambda: [])
        test_cmd: list[str] = Field(default_factory=lambda: ["make", "test"])
        env: dict[str, str] = Field(default_factory=dict)
        working_dir: str | None = None

    def _run(self, args: Args, *, verbose: bool = False) -> PluginOutput:
        task_dir = Path(args.task_dir)
        working_dir = Path(args.working_dir) if args.working_dir else task_dir
        
        if not task_dir.exists():
            raise PluginExecutionFailed(f"Task directory does not exist: {task_dir}")
        
        import os
        env = os.environ.copy()
        env.update(args.env)
        env["CLICOLOR_FORCE"] = "1"
        
        if args.build_cmd:
            print_info(f"Building: {' '.join(args.build_cmd)}", color="orange")
            try:
                result = subprocess.run(
                    args.build_cmd,
                    cwd=working_dir,
                    timeout=args.timeout,
                    capture_output=True,
                    text=True,
                    env=env,
                )
                if verbose and result.stdout:
                    print_info(result.stdout)
                if result.returncode != 0:
                    output = result.stdout + result.stderr
                    print_info(output, color="red")
                    raise PluginExecutionFailed(f"Build failed with code {result.returncode}")
                print_info("Build OK", color="green")
            except subprocess.TimeoutExpired:
                raise PluginExecutionFailed(f"Build timed out after {args.timeout}s")
        
        print_info(f"Testing: {' '.join(args.test_cmd)}", color="orange")
        try:
            result = subprocess.run(
                args.test_cmd,
                cwd=working_dir,
                timeout=args.timeout,
                capture_output=True,
                text=True,
                env=env,
            )

            if result.stdout:
                print_info(result.stdout)
            
            if result.returncode != 0:
                if result.stderr:
                    print_info(result.stderr, color="red")
                raise PluginExecutionFailed(
                    f"Tests failed with code {result.returncode}",
                )
            
            print_info("Tests OK", color="green")

            return PluginOutput(
                output="All tests passed",
                percentage=1.0,
            )
        except subprocess.TimeoutExpired:
            raise PluginExecutionFailed(f"Tests timed out after {args.timeout}s")
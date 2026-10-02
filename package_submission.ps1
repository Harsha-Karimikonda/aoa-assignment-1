$scriptDir = Split-Path -Parent $MyInvocation.MyCommand.Path
if (!$scriptDir) { $scriptDir = "C:\Users\hkarimkonda\.gemini\antigravity\scratch\aoa-assignment-1" }

$stagingDir = Join-Path $scriptDir "submission_staging"
$zipPath = Join-Path $scriptDir "Karimikonda Harsha assignment1.zip"

if (Test-Path $stagingDir) { Remove-Item -Recurse -Force $stagingDir }
New-Item -ItemType Directory -Path $stagingDir -Force | Out-Null

# Copy core source files with standard names
Copy-Item (Join-Path $scriptDir "include\graph.h") $stagingDir
Copy-Item (Join-Path $scriptDir "src\graph.cpp") $stagingDir
Copy-Item (Join-Path $scriptDir "include\graph_operations.h") $stagingDir
Copy-Item (Join-Path $scriptDir "src\graph_operations.cpp") $stagingDir
Copy-Item (Join-Path $scriptDir "include\graph_simulator.h") $stagingDir
Copy-Item (Join-Path $scriptDir "src\graph_simulator.cpp") $stagingDir
Copy-Item (Join-Path $scriptDir "src\simulated_test.cpp") $stagingDir
Copy-Item (Join-Path $scriptDir "include\realgraph_make.h") $stagingDir
Copy-Item (Join-Path $scriptDir "src\realgraph_make.cpp") $stagingDir
Copy-Item (Join-Path $scriptDir "src\run_realgraph_make.cpp") $stagingDir
Copy-Item (Join-Path $scriptDir "include\benchmark_utils.h") $stagingDir
Copy-Item (Join-Path $scriptDir "results.txt") $stagingDir
Copy-Item (Join-Path $scriptDir "README.md") $stagingDir
Copy-Item (Join-Path $scriptDir "Makefile") $stagingDir
Copy-Item (Join-Path $scriptDir "CMakeLists.txt") $stagingDir

# Create space-named aliases to guarantee 100% compliance with exact prompt naming
Copy-Item (Join-Path $scriptDir "include\graph_operations.h") (Join-Path $stagingDir "graph operations.h")
Copy-Item (Join-Path $scriptDir "src\graph_operations.cpp") (Join-Path $stagingDir "graph operations.cpp")
Copy-Item (Join-Path $scriptDir "include\graph_simulator.h") (Join-Path $stagingDir "graph simulator.h")
Copy-Item (Join-Path $scriptDir "src\graph_simulator.cpp") (Join-Path $stagingDir "graph simulator.cpp")
Copy-Item (Join-Path $scriptDir "src\simulated_test.cpp") (Join-Path $stagingDir "simulated test.cpp")
Copy-Item (Join-Path $scriptDir "include\realgraph_make.h") (Join-Path $stagingDir "realgraph make.h")
Copy-Item (Join-Path $scriptDir "src\realgraph_make.cpp") (Join-Path $stagingDir "realgraph make.cpp")
Copy-Item (Join-Path $scriptDir "src\run_realgraph_make.cpp") (Join-Path $stagingDir "run realgraph make.cpp")

# Create zip archive
if (Test-Path $zipPath) { Remove-Item -Force $zipPath }
Compress-Archive -Path "$stagingDir\*" -DestinationPath $zipPath -Force
Remove-Item -Recurse -Force $stagingDir

Write-Host "Submission archive generated successfully at: $zipPath"
Get-Item $zipPath | Select-Object Name, Length, LastWriteTime

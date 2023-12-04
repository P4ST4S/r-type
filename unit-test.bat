cmake ./tests/ -B tests/build -G "MinGW Makefiles"
cmake --build tests/build -j32

.\tests\build\Tests.exe

@REM gcovr -e tests
@REM gcovr -e tests -b

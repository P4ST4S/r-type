find ./ -type f -name "*.gcda" -delete

(echo -e "\033[34m"
echo -e -------------
echo -e BUILDING
echo -e -------------
echo -e "\033[00m"
cmake ./tests/ -B tests/build
cmake --build tests/build -j32) &&

(
    (
        echo -e "\033[0;35m"
        echo -e -------------
        echo -e RUNNING TESTS
        echo -e -------------
        echo -e "\033[00m"
        ./tests/build/Tests
    ) &&
    (
        echo -e "\033[0;33m"
        echo -e -------------
        echo -e COVERAGE "\033[00m"
        gcovr -e tests ;
        echo -e "\033[0;33m"
        echo -e -------------
        echo -e BRANCHES "\033[00m"
        gcovr -e tests -b
    )
)

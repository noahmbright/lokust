rm -rf build
CC=$(brew --prefix llvm)/bin/clang CXX=$(brew --prefix llvm)/bin/clang++ cmake -S . -B build -DLLVM_DIR=/opt/homebrew/Cellar/llvm/19.1.3/lib/cmake/llvm/ 
make -C build

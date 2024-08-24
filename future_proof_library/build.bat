g++ -shared -std=c++20 -DFUTURE_PROOF_LIBRARY_VERSION=2 future_proof_library.cpp -o future_proof_library.dll
g++ -DFUTURE_PROOF_LIBRARY_VERSION=2 -std=c++20 -L. -l:future_proof_library.dll consumer.cpp -o consumer

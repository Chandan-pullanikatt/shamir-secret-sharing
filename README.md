hamir's Secret Sharing in C++
This project demonstrates an implementation of Shamir's Secret Sharing Scheme using C++. It enables securely dividing a secret into n parts, where only a minimum of k parts are needed to retrieve the original secret.
What is Shamir’s Secret Sharing?
Shamir’s scheme is a cryptographic technique that breaks a secret into multiple pieces. The key idea is:
You can define a threshold k (minimum number of parts required).
If you have fewer than k, the secret cannot be reconstructed.
It relies on polynomial interpolation and modular arithmetic.
 How to Compile and Run
Make sure you have a C++ compiler like g++ installed.

g++ shamir.cpp -o shamir
./shamir
This will compile and run the program.

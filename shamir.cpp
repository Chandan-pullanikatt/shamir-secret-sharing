#include <iostream>
#include <vector>
#include <ctime>
#include <cstdlib>

using namespace std;

// A large prime number to use as the modulus (must be > secret)
const int PRIME = 9973;

// Structure for each share (x, y)
struct Share {
    int x;
    int y;
};

// Modular inverse using Extended Euclidean Algorithm
int modInverse(int a, int m) {
    int m0 = m, y = 0, x = 1;
    if (m == 1) return 0;

    while (a > 1) {
        int q = a / m;
        int t = m;

        m = a % m, a = t;
        t = y;
        y = x - q * y;
        x = t;
    }
    return x < 0 ? x + m0 : x;
}

// Function to evaluate polynomial at given x
int evaluatePolynomial(const vector<int>& coeffs, int x) {
    int result = 0;
    int power = 1;

    for (int coeff : coeffs) {
        result = (result + coeff * power) % PRIME;
        power = (power * x) % PRIME;
    }
    return result;
}

// Generate n shares from secret using polynomial of degree k-1
vector<Share> generateShares(int secret, int n, int k) {
    vector<int> coeffs;
    coeffs.push_back(secret); // constant term

    // Generate k-1 random coefficients
    for (int i = 1; i < k; ++i) {
        coeffs.push_back(rand() % PRIME);
    }

    vector<Share> shares;
    for (int i = 1; i <= n; ++i) {
        shares.push_back({i, evaluatePolynomial(coeffs, i)});
    }
    return shares;
}

// Reconstruct the secret using Lagrange Interpolation
int reconstructSecret(const vector<Share>& shares) {
    int secret = 0;

    for (size_t i = 0; i < shares.size(); ++i) {
        int xi = shares[i].x;
        int yi = shares[i].y;
        int li = 1;

        for (size_t j = 0; j < shares.size(); ++j) {
            if (i != j) {
                int xj = shares[j].x;
                int num = (0 - xj + PRIME) % PRIME;
                int den = (xi - xj + PRIME) % PRIME;
                li = (li * num % PRIME) * modInverse(den, PRIME) % PRIME;
            }
        }

        secret = (secret + yi * li % PRIME) % PRIME;
    }
    return secret;
}

int main() {
    srand(time(0));

    int secret, n, k;
    cout << "Enter secret (as a number less than " << PRIME << "): ";
    cin >> secret;
    cout << "Enter total number of shares to generate (n): ";
    cin >> n;
    cout << "Enter minimum shares required to reconstruct (k): ";
    cin >> k;

    if (secret >= PRIME || k > n || secret < 0) {
        cerr << "Invalid input. Make sure 0 ≤ secret < " << PRIME << " and k ≤ n." << endl;
        return 1;
    }

    // Generate and display shares
    vector<Share> shares = generateShares(secret, n, k);
    cout << "\nGenerated Shares:\n";
    for (const auto& s : shares) {
        cout << "(" << s.x << ", " << s.y << ")\n";
    }

    // Reconstruct secret from first k shares
    vector<Share> selectedShares(shares.begin(), shares.begin() + k);
    int recovered = reconstructSecret(selectedShares);

    cout << "\nReconstructed Secret using first " << k << " shares: " << recovered << endl;
    return 0;
}

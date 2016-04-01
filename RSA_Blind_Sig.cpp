#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <cmath>
#include <functional>
using namespace std;
typedef long long int ll;
ll r;
ll gcd(ll a, ll b) {
    return b == 0 ? a : gcd(b, a % b);
}
ll modInverse(ll a, ll m)
{
    a = a%m;
    for (ll x=1; x<m; x++)
       if ((a*x) % m == 1)
          return x;
}
ll random_prime(ll n)
{
    bool prime[n+1];
    memset(prime, true, sizeof(prime));
    for (ll p=2; p*p<=n; p++)
    {
        if (prime[p] == true)
        {
            for (ll i=p*2; i<=n; i += p)
                prime[i] = false;
        }
    }
    vector<ll> primes;
    for (int p=2; p<=n; p++)
       if (prime[p])
          primes.push_back(p);
    return primes[rand()%primes.size()];
}
ll random_coprime(ll n)
{
    vector<ll> coprimes;
    for(ll i=2;i<n;i++)
    {
        if(gcd(i,n)==1)
        {
            coprimes.push_back(i);
        }
    }
    return coprimes[rand()%coprimes.size()];
}
ll blinding(ll m,ll n,ll e)
{
    r=random_coprime(n);
    //r=2;
    cout << "r=" << r << endl;
    ll blinding_factor=1;
    while(e>0)
    {
        blinding_factor=(blinding_factor*r)%n;
        e--;
    }
    cout << "Blinding Factor: " << blinding_factor << endl;
    return (m*blinding_factor)%n;
}
ll signing(ll m,ll n, ll d)
{
    ll blinding_signature=1;
    while(d>0)
    {
        blinding_signature=(blinding_signature*m)%n;
        d--;
    }
    return blinding_signature;
}
ll rsa_signature(ll blinded_signature, ll r,ll n)
{
    ll rsa_sign=(blinded_signature*modInverse(r,n))%n;
   // cout << modInverse(r,n) << endl;
    return rsa_sign;
}
vector<ll> primeFactors(int n)
{
    vector<ll> prime_factors;
    while (n%2 == 0)
    {
        prime_factors.push_back(2);
        n = n/2;
    }
    for (int i = 3; i <= sqrt(n); i = i+2)
    {
        while (n%i == 0)
        {
            prime_factors.push_back(i);
            n = n/i;
        }
    }
    if (n > 2)
        prime_factors.push_back(n);
    return prime_factors;
}
ll crt(ll l, ll m,ll n)
{
    vector<ll> prime_fact=primeFactors(n);
    vector<ll> N,y,a,x_coe,k;
    for(ll i=0;i<prime_fact.size();i++)
    {
        N.push_back(n/prime_fact[i]);
    }
    for(int i=0;i<prime_fact.size();i++)
    {
        y.push_back(modInverse(N[i],prime_fact[i]));
    }
    for(int i=0;i<prime_fact.size();i++)
    {
        x_coe.push_back(l%prime_fact[i]);
    }
    for(int i=0;i<prime_fact.size();i++)
    {
        k.push_back(m%prime_fact[i]);
    }
    for(int i=0;i<prime_fact.size();i++)
    {
        a.push_back((modInverse(x_coe[i],prime_fact[i])*k[i])%prime_fact[i]);
    }
    ll ans=0;
    for(int i=0;i<prime_fact.size();i++)
    {
        ans+=(a[i]*N[i]*y[i]);
    }
    return ans%n;
}
int main()
{
    ll p,q;
    srand(time(NULL));
    p=random_prime(100);
    q=random_prime(100);
    while(p==q)
    {
        q=random_prime(100);
    }
   // p=5;q=7;
    cout << "p=" << p << " q=" << q << endl;
    ll n=p*q;
    cout << "n=" << n << endl;
    ll phin=(p-1)*(q-1);
    cout << "phin=" << phin << endl;
    ll e=random_coprime(phin);
    //e=5;
    cout << "e=" << e <<endl;
    ll d=modInverse(e,phin);
    cout << "d=" << d << endl;
    ll m;
    cout << "Enter Message: ";
    cin >> m;
    hash<ll> ll_hash;
   // cout << "Hashed value of " << m << " is " << ll_hash(m)<< endl;
    ll blind_msg = blinding(ll_hash(m),n,e);
    cout << "Blind Message: " << blind_msg << endl;
    ll blinded_signature=signing(blind_msg,n,d);
    cout << "Blinded Signature: " << blinded_signature << endl;
    ll rsa_sign=rsa_signature(blinded_signature,r,n);
    cout << "RSA Signature: " << rsa_sign << endl;
    ll k=e,msg=1;
    while(k>0)
    {
        msg=(msg*rsa_sign)%n;
        k--;
    }
    cout << "Hashed Message=" << msg << endl;
    if(msg==ll_hash(m))
        cout << "Valid Message" << endl;
    else
        cout << "Invalid Message" << endl;
    return 0;
}

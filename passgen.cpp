// enforce_with_inline_username.cpp
#include <bits/stdc++.h>
using namespace std;

random_device rd;
mt19937 gen(rd());

char rand_from(const string &s)
{
    uniform_int_distribution<size_t> d(0, s.size() - 1);
    return s[d(gen)];
}

string rand_chars(size_t n, const string &pool)
{
    string out;
    out.reserve(n);
    for (size_t i = 0; i < n; ++i)
        out.push_back(rand_from(pool));
    return out;
}

string lowercase(string s)
{
    for (char &c : s)
        c = tolower((unsigned char)c);
    return s;
}

bool contains_ci(const string &text, const string &pat)
{
    if (pat.empty())
        return false;
    string a = lowercase(text), b = lowercase(pat);
    return a.find(b) != string::npos;
}

bool check_policy(const string &pw, const string &username, int minL = 8, int maxL = 16)
{
    if ((int)pw.size() < minL || (int)pw.size() > maxL)
        return false;
    if (contains_ci(pw, username))
        return false;
    bool hasLower = false, hasUpper = false, hasDigit = false, hasSpec = false;
    string specials = "!@#$%&*?-_+=";
    for (char c : pw)
    {
        if (islower((unsigned char)c))
            hasLower = true;
        else if (isupper((unsigned char)c))
            hasUpper = true;
        else if (isdigit((unsigned char)c))
            hasDigit = true;
        else if (specials.find(c) != string::npos)
            hasSpec = true;
    }
    return hasLower && hasUpper && hasDigit && hasSpec;
}

string enforce(string candidate, const string &username, int minL = 8, int maxL = 16)
{
    const string lowers = "abcdefghijklmnopqrstuvwxyz";
    const string uppers = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    const string digits = "0123456789";
    const string specials = "!@#$%&*?-_+=";
    const string pool_all = lowers + uppers + digits + specials;

    auto remove_username = [&](string &s)
    {
        if (username.empty())
            return;
        string low_s = lowercase(s);
        string low_u = lowercase(username);
        size_t pos = 0;
        while ((pos = low_s.find(low_u, pos)) != string::npos)
        {
            for (size_t k = 0; k < low_u.size() && pos + k < s.size(); ++k)
                s[pos + k] = rand_from(pool_all);
            low_s = lowercase(s);
            pos += 1;
        }
    };

    int attempts = 0;
    while (!check_policy(candidate, username, minL, maxL) && attempts < 500)
    {
        remove_username(candidate);

        if ((int)candidate.size() > maxL)
        {
            candidate = candidate.substr(0, maxL);
            remove_username(candidate);
        }
        if ((int)candidate.size() < minL)
        {
            candidate += rand_chars(minL - candidate.size(), pool_all);
        }

        bool hasLower = false, hasUpper = false, hasDigit = false, hasSpec = false;
        for (char c : candidate)
        {
            if (islower((unsigned char)c))
                hasLower = true;
            else if (isupper((unsigned char)c))
                hasUpper = true;
            else if (isdigit((unsigned char)c))
                hasDigit = true;
            else if (specials.find(c) != string::npos)
                hasSpec = true;
        }

        vector<int> idx(candidate.size());
        iota(idx.begin(), idx.end(), 0);
        shuffle(idx.begin(), idx.end(), gen);
        size_t ix = 0;

        if (!hasLower && ix < idx.size())
            candidate[idx[ix++]] = rand_from(lowers);
        if (!hasUpper && ix < idx.size())
            candidate[idx[ix++]] = rand_from(uppers);
        if (!hasDigit && ix < idx.size())
            candidate[idx[ix++]] = rand_from(digits);
        if (!hasSpec && ix < idx.size())
            candidate[idx[ix++]] = rand_from(specials);

        if ((int)candidate.size() < minL)
            candidate += rand_chars(minL - candidate.size(), pool_all);

        shuffle(candidate.begin(), candidate.end(), gen);

        if (contains_ci(candidate, username))
        {
            size_t p = lowercase(candidate).find(lowercase(username));
            for (size_t k = 0; k < username.size() && p + k < candidate.size(); ++k)
                candidate[p + k] = rand_from(pool_all);
        }

        ++attempts;
    }

    if (!check_policy(candidate, username, minL, maxL))
    {
        string pw;
        pw += rand_from("abcdefghijklmnopqrstuvwxyz");
        pw += rand_from("ABCDEFGHIJKLMNOPQRSTUVWXYZ");
        pw += rand_from("0123456789");
        pw += rand_from("!@#$%&*?-_+=");
        int remaining = max(minL, 8) - 4;
        pw += rand_chars(remaining, pool_all);
        shuffle(pw.begin(), pw.end(), gen);
        candidate = pw;
        if (contains_ci(candidate, username))
            candidate = "A1!abcdef";
    }

    return candidate;
}

int main(int argc, char **argv)
{
    // EDIT THIS VARIABLE to set the username inside the code.
    const string USERNAME = "edit_me";

    string candidate;
    if (argc >= 2)
        candidate = argv[1];
    else
    {
        cout << "Enter candidate password: ";
        if (!getline(cin, candidate))
            return 1;
    }

    string out = enforce(candidate, USERNAME, 8, 16);
    cout << out << '\n';
    return 0;
}

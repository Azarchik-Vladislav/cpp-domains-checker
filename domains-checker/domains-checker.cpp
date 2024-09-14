#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>

using namespace std;

class Domain {
public:
    Domain() = default;

    Domain(string domain) : domain_ (domain.rbegin(), domain.rend()) {
        domain_ .push_back('.');
    }

    string GetDomain() {
        return domain_;
    }

    bool operator==(const Domain& other) const {
        return domain_ == other.domain_;
    }

    bool operator<(const Domain& other) const{
        return domain_ < other.domain_;
    }

    bool IsSubdomain(const Domain& other) const {
        if(domain_.substr(0, other.domain_.size()) == other.domain_) {
            return true;
        }

        return false;
    } 
private:
    string domain_;
};

class DomainChecker {
public:
    template <typename InIter>
    DomainChecker (InIter begin, InIter end) : forbidden_domains_(begin, end) {
        sort(forbidden_domains_.begin(), forbidden_domains_.end());
        
        auto not_unique_domains = unique(forbidden_domains_.begin(), forbidden_domains_.end(), 
                                         [](const Domain& lhs, const Domain& rhs) {
                                            return rhs.IsSubdomain(lhs);
                                          });

        forbidden_domains_.erase(not_unique_domains, forbidden_domains_.end());
    }

    bool IsForbidden(const Domain& domain) {
        auto forbidden_domain = upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain);

        if(forbidden_domain == forbidden_domains_.begin()) {
            return false;
        } else {
            return  domain.IsSubdomain(*prev(forbidden_domain));
        }
    }
private:
    vector<Domain> forbidden_domains_;
};

vector<Domain> ReadDomains(istream& input, size_t count_queries) {
    vector<Domain> domains;
    domains.reserve(count_queries);

    string domain;
    for(size_t i = 0; i < count_queries; ++i) {
        getline(input, domain);
        domains.push_back(domain);
    }

    return domains;
}

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

int main() {
    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}
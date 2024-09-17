#include <algorithm>
#include <cassert>
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
        return domain_.substr(0, other.domain_.size()) == other.domain_ ? true : false; //зачем тернарная операция? 
                                //достаточно просто вернуть domain_.substr(0, other.domain_.size()) == other.domain_ Результат этой операции - булево значение.
                                // bool result = domain_.substr(0, other.domain_.size()) == other.domain_; Но лишняя перменная не нужна. Поэтому можно вернуть выражение
                                // Иначе масло маслянное получается.
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

    const vector<Domain>& GetForbiddenDomains() {
        return forbidden_domains_;
    }

    bool IsForbidden(const Domain& domain) {
        auto forbidden_domain = upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain);

        return  !(forbidden_domain == forbidden_domains_.begin()) //тоже самое
                && domain.IsSubdomain(*prev(forbidden_domain)) ? true : false;
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

void TestCorrectSubdomainDefinition() {
    Domain domain_1("free.gdz.ru"s);

    { 
        Domain domain_2("freegdz.ru"s);
        Domain domain_3("math.freegdz.ru");

        assert(!domain_1.IsSubdomain(domain_2));
        assert(!domain_3.IsSubdomain(domain_1));

        assert(!domain_2.IsSubdomain(domain_1));
    }

    {
        Domain domain_2("gdz.ru"s);
        Domain domain_3("math.free.gdz.ru");
        assert(domain_3.IsSubdomain(domain_1));
        assert(domain_1.IsSubdomain(domain_2));
        assert(domain_3.IsSubdomain(domain_2));
    }
}

void TestSortAndUniqueDomains() {
    {
        string domains = "gdz.ru m.gdz.ru free.gdz.ru com\n"s;
        istringstream str(domains);

        istringstream count("2");

        vector<Domain> forbidden_domains = ReadDomains(str, ReadNumberOnLine<size_t>(count));
        DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
        assert(checker.GetForbiddenDomains().size() == 2);
    }

}

void TestCheckForbiddenDomains() {
    string domains = "gdz.ru\nm.gdz.ru\nfree.gdz.ru\ncom"s;
    istringstream str(domains);

    istringstream count("4");

    vector<Domain> forbidden_domains = ReadDomains(str, ReadNumberOnLine<size_t>(count));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());
    {
        Domain domain_1("map.com"s);
        Domain domain_2("gdz.ru"s);
        Domain domain_3("ru"s);

        assert(checker.IsForbidden(domain_1));
        assert(checker.IsForbidden(domain_2));

        assert(!checker.IsForbidden(domain_3));
    }
}

int main() {
    TestCorrectSubdomainDefinition();
    TestSortAndUniqueDomains();
    TestCheckForbiddenDomains();
    return 0;
}

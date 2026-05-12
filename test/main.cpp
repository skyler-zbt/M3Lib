import std;

import m3.vector;

int main(){
    m3::Vec<1,float> a{2.0f};

    std::println("{}", static_cast<const void*>(&a.x()));
    std::println("{}", static_cast<const void*>(&a.r()));
    std::println("{}", static_cast<const void*>(&a.s()));

    return 0;
}

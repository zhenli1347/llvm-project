// RUN: %clang_cc1 -verify -fopenmp -fopenmp-version=45 %s -Wuninitialized
// RUN: %clang_cc1 -verify -fopenmp %s -Wuninitialized

// RUN: %clang_cc1 -verify -fopenmp-simd -fopenmp-version=45 %s -Wuninitialized
// RUN: %clang_cc1 -verify -fopenmp-simd %s -Wuninitialized

void foo() {
}

bool foobool(int argc) {
  return argc;
}
extern int omp_default_mem_alloc;
struct S1; // expected-note 2 {{declared here}} expected-note 3 {{forward declaration of 'S1'}}
extern S1 a;
class S2 {
  mutable int a;

public:
  S2() : a(0) {}
  S2(S2 &s2) : a(s2.a) {}
  const S2 &operator =(const S2&) const;
  S2 &operator =(const S2&);
  static float S2s; // expected-note {{static data member is predetermined as shared}}
  static const float S2sc; // expected-note {{'S2sc' declared here}}
};
const float S2::S2sc = 0;
const S2 b;
const S2 ba[5];
class S3 {
  int a;
  S3 &operator=(const S3 &s3); // expected-note {{implicitly declared private here}}

public:
  S3() : a(0) {}
  S3(S3 &s3) : a(s3.a) {}
};
const S3 c;         // expected-note {{'c' defined here}}
const S3 ca[5];     // expected-note {{'ca' defined here}}
extern const int f; // expected-note {{'f' declared here}}
class S4 {
  int a;
  S4();             // expected-note 3 {{implicitly declared private here}}
  S4(const S4 &s4);

public:
  S4(int v) : a(v) {}
};
class S5 {
  int a;
  S5() : a(0) {} // expected-note {{implicitly declared private here}}

public:
  S5(const S5 &s5) : a(s5.a) {}
  S5(int v) : a(v) {}
};
class S6 {
  int a;
  S6() : a(0) {} // expected-note {{implicitly declared private here}}

public:
  S6(const S6 &s6) : a(s6.a) {}
  S6(int v) : a(v) {}
};

S3 h;
#pragma omp threadprivate(h) // expected-note 2 {{defined as threadprivate or thread local}}

template <class I, class C>
int foomain(int argc, char **argv) {
  I e(4);
  I g(5);
  int i;
  int &j = i;
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate // expected-error {{expected '(' after 'lastprivate'}}
  for (int k = 0; k < argc; ++k)
    ++k;
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate( // expected-error {{expected expression}} expected-error {{expected ')'}} expected-note {{to match this '('}}
  for (int k = 0; k < argc; ++k)
    ++k;
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate() // expected-error {{expected expression}}
  for (int k = 0; k < argc; ++k)
    ++k;
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(argc // expected-error {{expected ')'}} expected-note {{to match this '('}}
  for (int k = 0; k < argc; ++k)
    ++k;
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(argc, // expected-error {{expected expression}} expected-error {{expected ')'}} expected-note {{to match this '('}}
  for (int k = 0; k < argc; ++k)
    ++k;
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(argc > 0 ? argv[1] : argv[2]) // expected-error {{expected variable name}}
  for (int k = 0; k < argc; ++k)
    ++k;
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(argc) allocate , allocate(, allocate(omp_default , allocate(omp_default_mem_alloc, allocate(omp_default_mem_alloc:, allocate(omp_default_mem_alloc: argc, allocate(omp_default_mem_alloc: argv), allocate(argv) // expected-error {{expected '(' after 'allocate'}} expected-error 2 {{expected expression}} expected-error 2 {{expected ')'}} expected-error {{use of undeclared identifier 'omp_default'}} expected-note 2 {{to match this '('}}
  for (int k = 0; k < argc; ++k)
    ++k;
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(conditional: argc) lastprivate(conditional: // expected-error 2 {{use of undeclared identifier 'conditional'}} expected-error {{expected ')'}} expected-note {{to match this '('}}
  for (int k = 0; k < argc; ++k)
    ++k;
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(S1) // expected-error {{'S1' does not refer to a value}}
  for (int k = 0; k < argc; ++k)
    ++k;
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(a, b) // expected-error {{lastprivate variable with incomplete type 'S1'}} expected-warning {{type 'const S2' is not trivially copyable and not guaranteed to be mapped correctly}}
  for (int k = 0; k < argc; ++k)
    ++k;
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(argv[1]) // expected-error {{expected variable name}}
  for (int k = 0; k < argc; ++k)
    ++k;
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(e, g) // expected-error 2 {{calling a private constructor of class 'S4'}} expected-warning 2 {{type 'S4' is not trivially copyable and not guaranteed to be mapped correctly}}
  for (int k = 0; k < argc; ++k)
    ++k;
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(h) // expected-error {{threadprivate or thread local variable cannot be lastprivate}}
  for (int k = 0; k < argc; ++k)
    ++k;

  int v = 0;
#pragma omp target
#pragma omp teams
  {
#pragma omp distribute parallel for lastprivate(i)
    for (int k = 0; k < argc; ++k) {
      i = k;
      v += i;
    }
  }
#pragma omp target
#pragma omp teams private(i)
#pragma omp distribute parallel for lastprivate(j)
  for (int k = 0; k < argc; ++k)
    ++k;
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(i)
  for (int k = 0; k < argc; ++k)
    ++k;
  return 0;
}

void bar(S4 a[2]) {
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(a)
  for (int i = 0; i < 2; ++i)
    foo();
}

namespace A {
double x;
#pragma omp threadprivate(x) // expected-note {{defined as threadprivate or thread local}}
}
namespace B {
using A::x;
}

int main(int argc, char **argv) {
  const int d = 5;       // expected-note {{'d' defined here}}
  const int da[5] = {0}; // expected-note {{'da' defined here}}
  S4 e(4);
  S5 g(5);
  S3 m;
  S6 n(2);
  int i;
  int &j = i;
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate // expected-error {{expected '(' after 'lastprivate'}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate( // expected-error {{expected expression}} expected-error {{expected ')'}} expected-note {{to match this '('}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate() // expected-error {{expected expression}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(argc // expected-error {{expected ')'}} expected-note {{to match this '('}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(argc, // expected-error {{expected expression}} expected-error {{expected ')'}} expected-note {{to match this '('}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(argc > 0 ? argv[1] : argv[2]) // expected-error {{expected variable name}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(argc)
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(S1) // expected-error {{'S1' does not refer to a value}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(a, b, c, d, f) // expected-error {{lastprivate variable with incomplete type 'S1'}} expected-error 1 {{const-qualified variable without mutable fields cannot be lastprivate}} expected-error 2 {{const-qualified variable cannot be lastprivate}} expected-error {{incomplete type 'S1' where a complete type is required}} expected-warning {{type 'const S2' is not trivially copyable and not guaranteed to be mapped correctly}} expected-warning {{type 'const S3' is not trivially copyable and not guaranteed to be mapped correctly}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(argv[1]) // expected-error {{expected variable name}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(2 * 2) // expected-error {{expected variable name}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(ba) // expected-warning {{type 'const S2[5]' is not trivially copyable and not guaranteed to be mapped correctly}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(ca) // expected-error {{const-qualified variable without mutable fields cannot be lastprivate}} expected-warning {{type 'const S3[5]' is not trivially copyable and not guaranteed to be mapped correctly}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(da) // expected-error {{const-qualified variable cannot be lastprivate}}
  for (i = 0; i < argc; ++i)
    foo();
  int xa;
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(xa) // OK
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(S2::S2s) // expected-error {{shared variable cannot be lastprivate}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(S2::S2sc) // expected-error {{const-qualified variable cannot be lastprivate}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for safelen(5) // expected-error {{unexpected OpenMP clause 'safelen' in directive '#pragma omp distribute parallel for'}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(e, g) // expected-error {{calling a private constructor of class 'S4'}} expected-error {{calling a private constructor of class 'S5'}} expected-warning {{type 'S4' is not trivially copyable and not guaranteed to be mapped correctly}} expected-warning {{type 'S5' is not trivially copyable and not guaranteed to be mapped correctly}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(m) // expected-error {{'operator=' is a private member of 'S3'}} expected-warning {{type 'S3' is not trivially copyable and not guaranteed to be mapped correctly}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(h) // expected-error {{threadprivate or thread local variable cannot be lastprivate}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(B::x) // expected-error {{threadprivate or thread local variable cannot be lastprivate}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for private(xa), lastprivate(xa) // expected-error {{private variable cannot be lastprivate}} expected-note {{defined as private}}
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(i)
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(xa)
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(xa)
  for (i = 0; i < argc; ++i)
    foo();
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(j)
  for (i = 0; i < argc; ++i)
    foo();
// expected-error@+3 {{firstprivate variable cannot be lastprivate}} expected-note@+3 {{defined as firstprivate}}
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for firstprivate(m) lastprivate(m) // expected-warning {{type 'S3' is not trivially copyable and not guaranteed to be mapped correctly}}
  for (i = 0; i < argc; ++i)
    foo();
// expected-error@+3 {{lastprivate variable cannot be firstprivate}} expected-note@+3 {{defined as lastprivate}}
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(n) firstprivate(n) // expected-error {{calling a private constructor of class 'S6'}} expected-warning {{type 'S6' is not trivially copyable and not guaranteed to be mapped correctly}}
  for (i = 0; i < argc; ++i)
    foo();
  static int si;
#pragma omp target
#pragma omp teams
#pragma omp distribute parallel for lastprivate(si) // OK
  for (i = 0; i < argc; ++i)
    si = i + 1;
  return foomain<S4, S5>(argc, argv); // expected-note {{in instantiation of function template specialization 'foomain<S4, S5>' requested here}}
}

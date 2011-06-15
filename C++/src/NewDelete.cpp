#include <cstdlib>
#include <iostream>

void* operator new(std::size_t size) throw (std::bad_alloc)
{
   std::cout << "global operator new()" << std::endl;
   return malloc(size);
}

void operator delete(void* ptr) throw ()
{
   std::cout << "global operator delete()" << std::endl;
   free(ptr);
}

class A
{
   public:
      virtual ~A(){};

      void* operator new(std::size_t size) throw (std::bad_alloc)
      {
         std::cout << "A::operator new()" << std::endl;
         return malloc(size);
      }

      void operator delete(void* ptr) throw ()
      {
         std::cout << "A::operator delete()" << std::endl;
         free(ptr);
      }
};

class B : public A
{
   public:
};

class C : public B
{
   public:

      void* operator new(std::size_t size) throw (std::bad_alloc)
      {
         std::cout << "C::operator new()" << std::endl;
         return malloc(size);
      }

      void operator delete(void* ptr) throw ()
      {
         std::cout << "C::operator delete()" << std::endl;
         free(ptr);
      }
};

int playground(int argc, const char* argv[])
{
   delete new A();
   delete new B();
   delete new C();

   ::delete ::new C();

   return 0;
}

int main(int argc, const char* argv[])
{
   playground(argc, argv);
   return 0;
}

#ifndef IATO_TESTFIXTURE_HPP
#define IATO_TESTFIXTURE_HPP

#include "Isa.hpp"

#include <string>
#include <vector>

namespace iato {

  /// Contextual addresses that fixtures can reference (stack, bsp, etc.)
  struct FixtureContext {
    t_octa entry = OCTA_0;
    t_octa stack = OCTA_0;
    t_octa bsp   = OCTA_0;
    t_octa arg   = OCTA_0;
    t_octa tls   = OCTA_0;
    t_octa gp    = OCTA_0;
  };

  /// Lightweight description of the register/memory mutations that should be
  /// applied before `_start` executes. Fixtures are intentionally simple so
  /// they can be version controlled with the binary artifacts.
  class TestFixture {
  public:
    struct FixtureValue;

    struct RegWrite {
      t_lreg        type;
      long          index;
      FixtureValue* value;
    };

    struct MemWrite {
      FixtureValue* address;
      FixtureValue* value;
      size_t        size;
    };

    /// Construct an empty fixture
    TestFixture ();
    /// Deep-copy constructor/assignment
    TestFixture (const TestFixture& other);
    TestFixture& operator= (const TestFixture& other);
    ~TestFixture ();

    /// Parse a fixture file from disk
    static TestFixture load (const std::string& path);

    /// @return true if no register or memory writes are specified
    bool empty (void) const;

    /// @return the register writes
    const std::vector<RegWrite>& regs (void) const;
    /// @return the memory writes
    const std::vector<MemWrite>& mems (void) const;

  public:
    /// Parsed literal or symbolic value reference
    struct FixtureValue {
      FixtureValue ();
      FixtureValue (const FixtureValue& other);
      FixtureValue& operator= (const FixtureValue& other);
      ~FixtureValue () = default;

      /// @return the resolved value given the runtime context
      t_octa resolve (const FixtureContext& ctx) const;

      /// Static helpers to build literal/symbolic values
      static FixtureValue literal (t_octa value);
      static FixtureValue symbolic (const std::string& symbol,
				    long long offset);

    private:
      bool        d_has_symbol;
      std::string d_symbol;
      long long   d_offset;
      t_octa      d_literal;
    };

  private:
    std::vector<RegWrite> d_regs;
    std::vector<MemWrite> d_mems;

    void release (void);
    static FixtureValue* dup (const FixtureValue* value);
    static std::vector<RegWrite> clone_regs (const std::vector<RegWrite>&);
    static std::vector<MemWrite> clone_mems (const std::vector<MemWrite>&);
  };
}

#endif


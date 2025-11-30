#include "Abi.hpp"
#include "Exception.hpp"
#include "TestFixture.hpp"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <unordered_map>

namespace iato {
  using namespace std;

  namespace {
    using RegisterNameMap = std::unordered_map<std::string, long>;

    string trim (const string& text) {
      auto beg = find_if_not (text.begin (), text.end (),
			      [] (unsigned char ch) { return isspace (ch); });
      auto end = find_if_not (text.rbegin (), text.rend (),
			      [] (unsigned char ch) { return isspace (ch); }).base ();
      if (beg >= end) return "";
      return string (beg, end);
    }

    vector<string> tokenize (const string& line) {
      vector<string> tokens;
      string token;
      istringstream iss (line);
      while (iss >> token) tokens.push_back (token);
      return tokens;
    }

    string lowercase (string value) {
      transform (value.begin (), value.end (), value.begin (),
		 [] (unsigned char c) { return std::tolower (c); });
      return value;
    }

    template <typename T> T parse_integral (const string& text, int base = 0);

    template <> long long parse_integral<long long> (const string& text, int base) {
      size_t idx = 0;
      long long value = stoll (text, &idx, base);
      if (idx != text.size ()) {
	throw Exception ("invalid integer value '" + text + "'");
      }
      return value;
    }

    template <> unsigned long long parse_integral<unsigned long long> (
	const string& text, int base) {
      size_t idx = 0;
      unsigned long long value = stoull (text, &idx, base);
      if (idx != text.size ()) {
	throw Exception ("invalid integer value '" + text + "'");
      }
      return value;
    }

    t_octa parse_literal (const string& token) {
      if (token.empty ()) throw Exception ("invalid literal value");
      if (token[0] == '-') {
	return static_cast<t_octa> (
	    parse_integral<long long> (token, 0));
      }
      return static_cast<t_octa> (
	  parse_integral<unsigned long long> (token, 0));
    }

    long long parse_offset (const string& token) {
      if (token.empty ()) throw Exception ("invalid offset");
      return parse_integral<long long> (token, 0);
    }

    t_lreg parse_type (const string& token) {
      string lower = lowercase (token);
      if (lower == "g" || lower == "greg" || lower == "gr") return GREG;
      if (lower == "f" || lower == "freg" || lower == "fr") return FREG;
      if (lower == "p" || lower == "preg" || lower == "pr") return PREG;
      if (lower == "b" || lower == "breg" || lower == "br") return BREG;
      if (lower == "a" || lower == "areg" || lower == "ar") return AREG;
      if (lower == "c" || lower == "creg" || lower == "cr") return CREG;
      if (lower == "ip" || lower == "iprg" || lower == "ipreg") return IPRG;
      throw Exception ("unknown register type '" + token + "'");
    }

    RegisterNameMap build_areg_map () {
      return {
	  {"ar.ccv",  AR_CCV},
	  {"ar.bsp",  AR_BSP},
	  {"ar.bsps", AR_BSPS},
	  {"ar.fpsr", AR_FPSR},
	  {"ar.rnat", AR_RNAT},
	  {"ar.unat", AR_UNAT}
      };
    }

    long parse_index (t_lreg type, const string& token) {
      string lower = lowercase (token);
      auto drop = [&] (char prefix) -> bool {
	return lower.size () > 1 && lower[0] == prefix;
      };
      switch (type) {
      case GREG:
	if (drop ('r')) {
	  return static_cast<long> (
	      parse_integral<long long> (token.substr (1), 0));
	}
	break;
      case FREG:
	if (drop ('f')) {
	  return static_cast<long> (
	      parse_integral<long long> (token.substr (1), 0));
	}
	break;
      case PREG:
	if (drop ('p')) {
	  return static_cast<long> (
	      parse_integral<long long> (token.substr (1), 0));
	}
	break;
      case BREG:
	if (drop ('b')) {
	  return static_cast<long> (
	      parse_integral<long long> (token.substr (1), 0));
	}
	break;
      case AREG: {
	static const RegisterNameMap areg_map = build_areg_map ();
	auto it = areg_map.find (lower);
	if (it != areg_map.end ()) return it->second;
	break;
      }
      case CREG:
	if (drop ('c')) {
	  return static_cast<long> (
	      parse_integral<long long> (token.substr (1), 0));
	}
	break;
      default:
	break;
      }
      return static_cast<long> (
	  parse_integral<long long> (token, 0));
    }

    TestFixture::FixtureValue parse_value_token (const string& token) {
      if (token.empty ()) throw Exception ("invalid value token");
      if (!isalpha (static_cast<unsigned char> (token[0]))) {
	return TestFixture::FixtureValue::literal (parse_literal (token));
      }
      size_t pos = token.find_first_of ("+-", 1);
      if (pos == string::npos) {
	return TestFixture::FixtureValue::symbolic (lowercase (token), 0);
      }
      long long offset = parse_offset (token.substr (pos));
	  return TestFixture::FixtureValue::symbolic (
	      lowercase (token.substr (0, pos)), offset);
    }

    size_t parse_size (const string& token) {
      long long value = parse_integral<long long> (token, 0);
      if (value != 1 && value != 2 && value != 4 && value != 8) {
	throw Exception ("memory write size must be 1, 2, 4, or 8 bytes");
      }
      return static_cast<size_t> (value);
    }
  }

  TestFixture::TestFixture () = default;

  TestFixture::TestFixture (const TestFixture& other) {
    d_regs = clone_regs (other.d_regs);
    d_mems = clone_mems (other.d_mems);
  }

  TestFixture& TestFixture::operator= (const TestFixture& other) {
    if (this == &other) return *this;
    release ();
    d_regs = clone_regs (other.d_regs);
    d_mems = clone_mems (other.d_mems);
    return *this;
  }

  TestFixture::~TestFixture () {
    release ();
  }

  void TestFixture::release (void) {
    for (auto& entry : d_regs) delete entry.value;
    for (auto& entry : d_mems) {
      delete entry.address;
      delete entry.value;
    }
    d_regs.clear ();
    d_mems.clear ();
  }

  TestFixture TestFixture::load (const string& path) {
    TestFixture fixture;
    ifstream input (path.c_str ());
    if (!input.is_open ()) {
      throw Exception ("unable to open fixture file '" + path + "'");
    }
    string line;
    size_t lineno = 0;
    while (getline (input, line)) {
      lineno++;
      string trimmed = trim (line);
      if (trimmed.empty () || trimmed[0] == '#') continue;
      vector<string> tokens = tokenize (trimmed);
      if (tokens.empty ()) continue;
      string verb = lowercase (tokens[0]);
      try {
	if (verb == "reg") {
	  if (tokens.size () != 4) {
	    throw Exception ("reg entries require 3 arguments");
	  }
	  t_lreg type = parse_type (tokens[1]);
	  long index  = parse_index (type, tokens[2]);
	  FixtureValue* value =
	      new FixtureValue (parse_value_token (tokens[3]));
	  fixture.d_regs.push_back ({type, index, value});
	} else if (verb == "mem") {
	  if (tokens.size () != 4) {
	    throw Exception ("mem entries require 3 arguments");
	  }
	  FixtureValue* address =
	      new FixtureValue (parse_value_token (tokens[1]));
	  FixtureValue* value =
	      new FixtureValue (parse_value_token (tokens[2]));
	  size_t size = parse_size (tokens[3]);
	  fixture.d_mems.push_back ({address, value, size});
	} else {
	  throw Exception ("unknown fixture directive '" + tokens[0] + "'");
	}
      } catch (const Exception& ex) {
	throw Exception (ex.gettype (),
			 "fixture " + path + ":" + to_string (lineno) +
			 ": " + ex.getreason ());
      }
    }
    return fixture;
  }

  bool TestFixture::empty (void) const {
    return d_regs.empty () && d_mems.empty ();
  }

  const vector<TestFixture::RegWrite>& TestFixture::regs (void) const {
    return d_regs;
  }

  const vector<TestFixture::MemWrite>& TestFixture::mems (void) const {
    return d_mems;
  }

  TestFixture::FixtureValue::FixtureValue () {
    d_has_symbol = false;
    d_symbol = "";
    d_offset = 0;
    d_literal = OCTA_0;
  }

  TestFixture::FixtureValue::FixtureValue (const FixtureValue& other) {
    d_has_symbol = other.d_has_symbol;
    d_symbol = other.d_symbol;
    d_offset = other.d_offset;
    d_literal = other.d_literal;
  }

  TestFixture::FixtureValue& TestFixture::FixtureValue::operator= (
      const FixtureValue& other) {
    if (this == &other) return *this;
    d_has_symbol = other.d_has_symbol;
    d_symbol = other.d_symbol;
    d_offset = other.d_offset;
    d_literal = other.d_literal;
    return *this;
  }

  TestFixture::FixtureValue
  TestFixture::FixtureValue::literal (t_octa value) {
    FixtureValue fv;
    fv.d_has_symbol = false;
    fv.d_literal = value;
    return fv;
  }

  TestFixture::FixtureValue
  TestFixture::FixtureValue::symbolic (const string& symbol,
				       long long offset) {
    FixtureValue fv;
    fv.d_has_symbol = true;
    fv.d_symbol = symbol;
    fv.d_offset = offset;
    return fv;
  }

  t_octa TestFixture::FixtureValue::resolve (const FixtureContext& ctx) const {
    if (d_has_symbol == false) return d_literal;
    t_octa base = OCTA_0;
    if (d_symbol == "stack") base = ctx.stack;
    else if (d_symbol == "bsp") base = ctx.bsp;
    else if (d_symbol == "arg") base = ctx.arg;
    else if (d_symbol == "tls") base = ctx.tls;
    else if (d_symbol == "gp") base = ctx.gp;
    else if (d_symbol == "entry") base = ctx.entry;
    else {
      throw Exception ("unknown fixture symbol '" + d_symbol + "'");
    }
    if (d_offset == 0) return base;
    if (d_offset > 0) {
      return base + static_cast<t_octa> (d_offset);
    }
    return base - static_cast<t_octa> (-d_offset);
  }

  TestFixture::FixtureValue*
  TestFixture::dup (const FixtureValue* value) {
    if (!value) return nullptr;
    return new FixtureValue (*value);
  }

  vector<TestFixture::RegWrite>
  TestFixture::clone_regs (const vector<RegWrite>& regs) {
    vector<RegWrite> clone;
    clone.reserve (regs.size ());
    for (const auto& entry : regs) {
      clone.push_back ({entry.type, entry.index, dup (entry.value)});
    }
    return clone;
  }

  vector<TestFixture::MemWrite>
  TestFixture::clone_mems (const vector<MemWrite>& mems) {
    vector<MemWrite> clone;
    clone.reserve (mems.size ());
    for (const auto& entry : mems) {
      clone.push_back (
	  {dup (entry.address), dup (entry.value), entry.size});
    }
    return clone;
  }
}


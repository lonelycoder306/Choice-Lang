#pragma once
#include <climits>
#include <cstdint>
#include <cstring>
#include <fstream>
#include <memory>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>

namespace Object
{
	// Basic type aliases.
	
	enum ObjType : uint8_t
	{
		OBJ_BASE,
		OBJ_INT,
		OBJ_UINT,
		OBJ_DEC,
		OBJ_BOOL,
		OBJ_STRING,
		OBJ_NULL,
		OBJ_INVALID
	};

	struct TypeMismatch // General type mismatch error class.
	{
		std::string_view message;
		ObjType first;
		ObjType second;
	};
	
	struct Base
	{   
		ObjType type;   // Object type.

		Base();
		virtual ~Base() = default;
		Base(ObjType type);

		virtual std::string print() = 0;
		virtual std::string printType();
		virtual void emit(std::ofstream& is);

		// virtual bool operator==(const Base& other) const;
		// virtual Base& operator+(const Base& other) const;
		// virtual Base& operator-(const Base& other) const;
	};

	using BaseUP = std::unique_ptr<Base>;

	struct Num : public Base
	{
		union value {
			int64_t i;
			uint64_t u;
			double d;
		} as;
	};
	
	struct Int : public Base
	{
		int64_t value;

		Int();
		Int(int64_t value);

		std::string print() override;
		std::string printType() override;
		void emit(std::ofstream& is) override;
	};

	struct UInt : public Base
	{
		uint64_t value;

		UInt();
		UInt(uint64_t value);

		std::string print() override;
		std::string printType() override;
		void emit(std::ofstream& is) override;
	};

	struct Dec : public Base
	{
		double value;

		Dec();
		Dec(double value);

		std::string print() override;
		std::string printType() override;
		void emit(std::ofstream& is) override;
	};

	struct Bool : public Base
	{
		bool value;

		Bool() = default;
		Bool(bool value);

		std::string print() override;
		std::string printType() override;
	};

	struct String : public Base
	{
		std::string_view value;
		std::string alt;

		String();
		String(std::string_view& value);

		// Factory method.
		static String makeString(const std::string& value);
		std::string print() override;
		std::string printType() override;
		void emit(std::ofstream& is) override;
	};

	struct List : public Base
	{

	};

	struct Table : public Base
	{

	};

	struct Null : public Base
	{
		Null();

		std::string print() override;
		std::string printType() override;
	};
}

#define IS_NUM(type) \
	((type == Object::OBJ_INT) || (type == Object::OBJ_UINT) \
	|| (type == Object::OBJ_DEC))

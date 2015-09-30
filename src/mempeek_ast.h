#ifndef __mempeek_ast_h__
#define __mempeek_ast_h__

#include <ostream>
#include <string>
#include <vector>

#include <stdint.h>

#ifdef ASTDEBUG
#include <iostream>
#endif


//////////////////////////////////////////////////////////////////////////////
// class ASTNode
//////////////////////////////////////////////////////////////////////////////

class ASTNode {
public:
	ASTNode() {}
	virtual ~ASTNode();

	void push_back( ASTNode* node );

	virtual uint64_t execute() = 0;

	static int get_default_size();

protected:
	typedef std::vector< ASTNode* > nodelist_t;

	const nodelist_t& get_children();

	static uint64_t parse_int( std::string str );

private:
	nodelist_t m_Children;

	ASTNode( const ASTNode& ) = delete;
	ASTNode& operator=( const ASTNode& ) = delete;
};


//////////////////////////////////////////////////////////////////////////////
// class ASTNodeBlock
//////////////////////////////////////////////////////////////////////////////

class ASTNodeBlock : public ASTNode {
public:
	ASTNodeBlock();

	uint64_t execute() override;
};


//////////////////////////////////////////////////////////////////////////////
// class ASTNodeWhile
//////////////////////////////////////////////////////////////////////////////

class ASTNodeWhile : public ASTNode {
public:
	ASTNodeWhile( ASTNode* condition, ASTNode* block );

	uint64_t execute() override;
};


//////////////////////////////////////////////////////////////////////////////
// class ASTNodePoke
//////////////////////////////////////////////////////////////////////////////

class ASTNodePoke : public ASTNode {
public:
	ASTNodePoke( ASTNode* address, ASTNode* value, int size_restriction );
	ASTNodePoke( ASTNode* address, ASTNode* value, ASTNode* mask, int size_restriction );

	uint64_t execute() override;

private:
	int m_SizeRestriction;
};


//////////////////////////////////////////////////////////////////////////////
// class ASTNodePrint
//////////////////////////////////////////////////////////////////////////////

class ASTNodePrint : public ASTNode {
public:
	enum {
		MOD_DEC = 0x01,
		MOD_HEX = 0x02,
		MOD_BIN = 0x03,
		MOD_NEG = 0x04,

		MOD_8BIT = 0x10,
		MOD_16BIT = 0x20,
		MOD_32BIT = 0x30,
		MOD_64BIT = 0x40,

		MOD_SIZEMASK = 0xf0,
		MOD_TYPEMASK = 0x0f
	};

	ASTNodePrint();
	ASTNodePrint( std::string text );
	ASTNodePrint( ASTNode* expression, int modifier );

	uint64_t execute() override;

	static int get_default_size();

private:
	void print_value( std::ostream& out, uint64_t value );

	int m_Modifier = MOD_DEC | MOD_32BIT;
	std::string m_Text = "";
};


//////////////////////////////////////////////////////////////////////////////
// class ASTNodeDef
//////////////////////////////////////////////////////////////////////////////

class ASTNodeDef : public ASTNode {
public:
	ASTNodeDef( std::string name, ASTNode* address );
	ASTNodeDef( std::string name, ASTNode* address, std::string from );

	uint64_t execute() override;
};


//////////////////////////////////////////////////////////////////////////////
// class ASTNodeRestriction
//////////////////////////////////////////////////////////////////////////////

class ASTNodeRestriction : public ASTNode {
public:
	ASTNodeRestriction( ASTNode* node, int size_restriction );

	uint64_t execute() override;

private:
	int m_SizeRestriction;
};


//////////////////////////////////////////////////////////////////////////////
// class ASTNodeVar
//////////////////////////////////////////////////////////////////////////////

class ASTNodeVar : public ASTNode {
public:
	ASTNodeVar( std::string name );
	ASTNodeVar( std::string name, ASTNode* index );

	uint64_t execute() override;
};


//////////////////////////////////////////////////////////////////////////////
// class ASTNodeConstant
//////////////////////////////////////////////////////////////////////////////

class ASTNodeConstant : public ASTNode {
public:
	ASTNodeConstant( std::string str );

	uint64_t execute() override;

private:
	uint64_t m_Value;
};


//////////////////////////////////////////////////////////////////////////////
// class ASTNode inline functions
//////////////////////////////////////////////////////////////////////////////

inline void ASTNode::push_back( ASTNode* node )
{
#ifdef ASTDEBUG
	std::cerr << "AST[" << this << "]: push back node=[" << node << "]" << std::endl;
#endif

	m_Children.push_back( node );
}

inline const ASTNode::nodelist_t& ASTNode::get_children()
{
	return m_Children;
}


#endif // __mempeek_ast_h__

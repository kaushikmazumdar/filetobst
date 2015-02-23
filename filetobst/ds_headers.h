// header file defining the structures
#include <stdbool.h>
typedef enum
{
	BOOK,
	PERIODICAL,
	ENCYCLOPEDIA
}BOOK_TYPE;

struct master
{
	BOOK_TYPE eBookType;
	char      cName[60];
	char      cAuthor[40];
	char      cEditor[40];
	char      cPublisher[40];
	float     fPrice;
	bool      bIsInStock;
	int       iVolNo;
};

struct treeNode
{
	struct master *node;
	struct treeNode *left;
	struct treeNode *right;
	struct treeNode *parent;
};

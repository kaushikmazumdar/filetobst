#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "ds_headers.h"

#define OUTPUT_FILE "outputbookdb.txt"
#define clear() printf("\033[H\033[J")

// Method defs
void Usage();
int FileNameLength(const char *);
struct master* ProcessLine(char*);
void TrimWhiteSpace(char**);
void ShowInfo(struct master*);
void PopulateStructure(char *, struct master **);
void PopulateBST(struct treeNode **, struct master *);
void GodMode();
void PunIntended();
void Delete(struct treeNode **, struct treeNode *);
void Reconstruct(struct treeNode *, FILE *);
void Inorder(struct treeNode *, FILE *);
void StartInteraction(struct treeNode *, FILE *);
struct master* Insert();
struct treeNode* MinNode(struct treeNode*);

int main(int argc, char *argv[])
{
	FILE *fp_in=NULL;
	FILE *fp_out=NULL;
	char *cInputFile=NULL;
	unsigned uiNameLen;
	struct treeNode *node=NULL;
	struct master *smRoot=NULL;

	// process inputs
	if(argc<2)
	{
		Usage();
		exit(-1);
	}
	else
	{
		// get the arguments
		int opt;
		while((opt = getopt(argc,argv,"f:")) != -1)
		{
			switch(opt)
			{
				case 'f':
					uiNameLen = FileNameLength(optarg);
					cInputFile = (char*)malloc(uiNameLen+1);
					memcpy(cInputFile,optarg,uiNameLen+1);
					break;
				case '?':
					Usage();
					break;
			}
		
		}
	}

	// all inputs set and done, proceed to file processing
	fp_in = fopen(cInputFile,"r");
	if( !fp_in )
		exit(-1);
	else
	{
		char *line=NULL;
		size_t lineLen;
		ssize_t read;

		// read and process a line
		while( (read = getline(&line,&lineLen,fp_in)) != -1)
		{
			smRoot = ProcessLine(line);	

			// Populate a BST
			PopulateBST(&node,smRoot);
		}
		fclose(fp_in);
	}

	// population blast done! All at the mercy of user now.
	StartInteraction(node,fp_out);

	return 0;
}
void StartInteraction(struct treeNode *rootNode, FILE *fp_out)
{
	struct treeNode *root = rootNode;
	struct master *smTemp = NULL;
	
	GodMode();
	while(1)
	{
		PunIntended();
		printf("Choose wisely: ");

		int iBookType;
		bool bInStock;
		int iChoice;
		scanf("%d",&iChoice);

		switch(iChoice)
		{
			case 1:
				ShowInfo(root->node);
				break;

			case 2:
				if(!root->left)
					printf("Nothing on left...\n");
				else
				{
					printf("Traversing left...\n");
					root = root->left;
				}
				break;

			case 3:
				if(!root->right)
					printf("Nothing on right...\n");
				else
				{
					printf("Traversing right...\n");
					root = root->right;
				}
				break;

			case 4:
				if(!root->parent)
					printf("Nothing on up...\n");
				else
				{
					printf("Traversing up...\n");
					root = root->parent;
				}
				break;

			case 5:
				smTemp = Insert();
				PopulateBST(&rootNode,smTemp);
				root = rootNode;
				break;

			case 6:
				Delete(&rootNode,root);
				root = rootNode;
				break;

			case 7:
				fp_out = fopen(OUTPUT_FILE,"w");
				Inorder(rootNode,fp_out);
				fclose(fp_out);
				break;

			case 8:
				exit(0);
				break;
			default:
				printf("Invalid choice.\n");
		}
	}
}
void Inorder(struct treeNode *tree, FILE *fp_out)
{
	// inorder traversal gives ascending order
	if(tree != NULL)
	{
		Inorder(tree->left,fp_out);
		Reconstruct(tree,fp_out);
		Inorder(tree->right,fp_out);
	}
}

void Reconstruct(struct treeNode *tree, FILE *fp_out)
{
	char cString[150];
	memset(cString,0x00,sizeof(cString));

	if(tree->node->eBookType == BOOK)
	{
		char cPrice[15];
		sprintf(cPrice,"%.2f",tree->node->fPrice);

		//type
		strcat(cString,"B");
		strcat(cString," # ");

		//name
		strcat(cString,tree->node->cName);
		strcat(cString," # ");

		//author
		strcat(cString,tree->node->cAuthor);
		strcat(cString," # ");

		//price
		strcat(cString,cPrice);
		strcat(cString," # ");

		//stock
		if(tree->node->bIsInStock)
			strcat(cString,"T");
		else
			strcat(cString,"F");		
	}
	else if(tree->node->eBookType == PERIODICAL)
	{
		char cPrice[15];
		char cVolNo[15];
		sprintf(cPrice,"%.2f",tree->node->fPrice);
		sprintf(cVolNo,"%d",tree->node->iVolNo);

		//type
		strcat(cString,"P");
		strcat(cString," # ");

		//name
		strcat(cString,tree->node->cName);
		strcat(cString," # ");

		//editor
		strcat(cString,tree->node->cEditor);
		strcat(cString," # ");

		//price
		strcat(cString,cPrice);
		strcat(cString," # ");

		//stock
		if(tree->node->bIsInStock)
			strcat(cString,"T");
		else
			strcat(cString,"F");		

		strcat(cString," # ");

		//vol no
		strcat(cString,cVolNo);
	}
	else if(tree->node->eBookType == ENCYCLOPEDIA)
	{
		char cPrice[15];
		sprintf(cPrice,"%.2f",tree->node->fPrice);

		//type
		strcat(cString,"E");
		strcat(cString," # ");

		//name
		strcat(cString,tree->node->cName);
		strcat(cString," # ");

		//publisher
		strcat(cString,tree->node->cPublisher);
		strcat(cString," # ");

		//price
		strcat(cString,cPrice);
		strcat(cString," # ");

		//stock
		if(tree->node->bIsInStock)
			strcat(cString,"T");
		else
			strcat(cString,"F");		
	}

	fprintf(fp_out,"%s\n",cString);
}

void Delete(struct treeNode **tree, struct treeNode *node)
{
	if ( (*tree) == NULL )
		return;

	// check left
	if ( strcmp( (*tree)->node->cName, node->node->cName ) > 0 )
		Delete(&((*tree)->left),node);
	else if( strcmp( (*tree)->node->cName, node->node->cName ) < 0 )
		Delete( &((*tree)->right),node);
	else if ( strcmp( (*tree)->node->cName, node->node->cName ) == 0 )
	{
		// that's your calling
		
		// leaf node
		if( ((*tree)->left == NULL) && ((*tree)->right == NULL) )
		{
			struct treeNode *temp = (*tree);
			
			if( temp->parent->left == temp )
				temp->parent->left = NULL;
			else if( temp->parent->right == temp )
				temp->parent->right = NULL;
			
			free(temp);
		}

		// node with one child, for sure
		else if ( (((*tree)->left != NULL) && ((*tree)->right == NULL)) ||
		     (((*tree)->right != NULL) && ((*tree)->left == NULL)) )
		{
			struct treeNode *temp = (*tree);

			// this node is right of parent?
			if( temp->parent->right == temp )
			{
				// and this node has right child?
				if ( temp->right != NULL )
				{
					temp->parent->right = temp->right;
					temp->right->parent = temp->parent;
				}
				// or is the left child?
				else if( temp->left != NULL )
				{
					temp->parent->right = temp->left;
					temp->left->parent = temp->parent;
				}
			}
			// or this node is the left of parent?
			if ( temp->parent->left == temp )
			{
				// and this node has right child?
				if ( temp->right != NULL )
				{
					temp->parent->left = temp->right;
					temp->right->parent = temp->parent;
				}
				// or is the left child?
				else if ( temp->left != NULL )
				{
					temp->parent->left == temp->left;
					temp->left->parent = temp->parent;
				}
			}

			free(temp);
		}

		// node having 2 children.
		else if ( ((*tree)->left != NULL) && ((*tree)->right != NULL) )
		{
			// tough luck.

			// get the inorder successor
			struct treeNode *temp = MinNode((*tree)->right);

			// copy inorder's node here.
			(*tree)->node = temp->node;

			// delete the inorder successor
			Delete(&((*tree)->right),temp);
		}
	}	
}

struct treeNode* MinNode(struct treeNode *root)
{
	struct treeNode *temp = root;

	while(temp->left != NULL)
		temp = temp->left;

	return temp;
}

struct master* Insert()
{

				struct master *smTemp = (struct master*)malloc(sizeof(struct master));
				int iBookType;
				bool bInStock;
				printf("\nType? 1. Book | 2. Periodical | 3. Encyclopedia << ");
				scanf("%d",&iBookType);
				getchar();

				if(iBookType == 1)
				{
					smTemp->eBookType = BOOK;
					
					printf("Name: ");
					fgets(smTemp->cName,sizeof(smTemp->cName),stdin);
					if( smTemp->cName[strlen(smTemp->cName)-1]=='\n' )
						smTemp->cName[strlen(smTemp->cName)-1] = '\0';

					printf("Author: ");
					fgets(smTemp->cAuthor,sizeof(smTemp->cAuthor),stdin);
					if( smTemp->cAuthor[strlen(smTemp->cAuthor)-1]=='\n' )
						smTemp->cAuthor[strlen(smTemp->cAuthor)-1] = '\0';
					
					printf("Price: ");
					scanf("%f",&(smTemp->fPrice));
					
					printf("InStock? 1. Yes | 2. No << ");
					scanf("%d",&bInStock);	
					if(bInStock==1)
						smTemp->bIsInStock = true;
					else if(bInStock==2)
						smTemp->bIsInStock = false;
				}
				else if(iBookType == 2)
				{
					smTemp->eBookType = PERIODICAL;

					printf("Name: ");
					fgets(smTemp->cName,sizeof(smTemp->cName),stdin);
					if( smTemp->cName[strlen(smTemp->cName)-1]=='\n' )
						smTemp->cName[strlen(smTemp->cName)-1] = '\0';

					printf("Editor: ");
					fgets(smTemp->cEditor,sizeof(smTemp->cEditor),stdin);
					if( smTemp->cEditor[strlen(smTemp->cEditor)-1]=='\n' )
						smTemp->cEditor[strlen(smTemp->cEditor)-1] = '\0';

					printf("Price: ");
					scanf("%f",&(smTemp->fPrice));
					
					printf("InStock? 1. Yes | 2. No << ");
					scanf("%d",&bInStock);
					if(bInStock==1)
						smTemp->bIsInStock = true;
					else if(bInStock==2)
						smTemp->bIsInStock = false;
					
					printf("VolNo: ");
					scanf("%d",&(smTemp->iVolNo));
				}
				else if(iBookType == 3)
				{
					smTemp->eBookType = ENCYCLOPEDIA;
					
					printf("Name: ");
					fgets(smTemp->cName,sizeof(smTemp->cName),stdin);
					if( smTemp->cName[strlen(smTemp->cName)-1]=='\n' )
						smTemp->cName[strlen(smTemp->cName)-1] = '\0';
					
					printf("Publisher: ");
					fgets(smTemp->cPublisher,sizeof(smTemp->cPublisher),stdin);
					if( smTemp->cPublisher[strlen(smTemp->cPublisher)-1]=='\n' )
						smTemp->cPublisher[strlen(smTemp->cPublisher)-1] = '\0';
					
					printf("Price: ");
					scanf("%f",&(smTemp->fPrice));
					printf("InStock? 1. Yes | 2. No << ");
					scanf("%d",&bInStock);
					if(bInStock==1)
						smTemp->bIsInStock = true;
					else if(bInStock==2)
						smTemp->bIsInStock = false;
				}

				return smTemp;
}
void GodMode()
{
	clear();
	printf("\n***************************************************************************\n\n");
	printf("  Welcome to the God mode!!!\n");
	printf("  You can be Vishnu, The Viewer (traverse left or right or up, and view).\n");
	printf("  You can be Brahma, The Creator (insert a node, and SAVE the world).\n");
	printf("  You can be Shiva, The Destroyer (delete a node).\n");
	printf("\n****************************************************************************\n");
}
void PunIntended()
{
	printf("\n---------\n");
	printf("1. Info\n");
	printf("2. Left\n");
	printf("3. Right\n");
	printf("4. Up\n");
	printf("5. Insert\n");
	printf("6. Delete\n");
	printf("7. Save\n");
	printf("8. Exit\n");
}


struct treeNode* newNode(struct master *node)
{
	struct treeNode *treeNode = (struct treeNode*)malloc(sizeof(struct treeNode));
	treeNode->node = node;
	treeNode->left = treeNode->right = treeNode->parent = NULL;

	return treeNode;
}

void PopulateBST(struct treeNode **root, struct master *node)
{
	if(*root==NULL)
	{
		// root node
		*root = newNode(node);
	}
	else
	{
		// not the root, do the usual BST
		if( strcmp((*root)->node->cName,node->cName) > 0 )
		{
			//left child
			PopulateBST( &((*root)->left),node );
			(*root)->left->parent = (*root);
		}
		else
		{
			//right child
			PopulateBST( &((*root)->right),node );
			(*root)->right->parent = (*root);
		}
	}
}

struct master* ProcessLine(char *cLine)
{
	struct master *smTemp;
	smTemp = (struct master*)malloc(sizeof(struct master));
	memset(smTemp,0,sizeof(struct master));

	// populate structure
	PopulateStructure(cLine,&smTemp);

	return smTemp;
}

void ShowInfo(struct master *smTemp)
{
	printf("\nShowing info...\n\n");	
	if(smTemp->eBookType==BOOK)
	{
		printf("Type: BOOK\n");
		printf("Name: %s\n",smTemp->cName);
		printf("Author: %s\n",smTemp->cAuthor);
		printf("Price: %.2f\n",smTemp->fPrice);
		printf("InStock?: %s\n",smTemp->bIsInStock?"True" : "False");
	}
	else if(smTemp->eBookType==PERIODICAL)
	{
		printf("Type: PERIODICAL\n");
		printf("Name: %s\n",smTemp->cName);
		printf("Editor: %s\n",smTemp->cEditor);
		printf("Price: %.2f\n",smTemp->fPrice);
		printf("InStock?: %s\n",smTemp->bIsInStock?"True" : "False");
		printf("VolNo: %d\n",smTemp->iVolNo);
	}
	else if(smTemp->eBookType==ENCYCLOPEDIA)
	{
		printf("Type: ENCYCLOPEDIA\n");
		printf("Name: %s\n",smTemp->cName);
		printf("Publisher: %s\n",smTemp->cPublisher);
		printf("Price: %.2f\n",smTemp->fPrice);
		printf("InStock?: %s\n",smTemp->bIsInStock?"True" : "False");
	}
}

void PopulateStructure(char *cLine, struct master **smTemp)
{
	//remove white spaces
	TrimWhiteSpace(&cLine);

	char cArray[8][60];
	char *pch=NULL;
	char *pLocal = cLine;

	unsigned int uiIter=0;

	pch = strtok(pLocal,"#");
	while(pch!=NULL)
	{
		// remove white spaces
		TrimWhiteSpace(&pch);

		sprintf(cArray[uiIter],"%s",pch);
		uiIter++;
		pch = strtok(NULL,"#");
	}

	// check for book type and fill the structure
	if(cArray[0][0]=='B')
	{
		(*smTemp)->eBookType = BOOK;
		strcpy((*smTemp)->cName,cArray[1]);
		strcpy((*smTemp)->cAuthor,cArray[2]);
		(*smTemp)->fPrice = atof(cArray[3]);
		if(cArray[4][0]=='T')
			(*smTemp)->bIsInStock = true;
		else if(cArray[4][0]=='F')
			(*smTemp)->bIsInStock = false;		
	}
	else if(cArray[0][0]=='P')
	{
		(*smTemp)->eBookType = PERIODICAL;
		strcpy((*smTemp)->cName,cArray[1]);
		strcpy((*smTemp)->cEditor,cArray[2]);
		(*smTemp)->fPrice = atof(cArray[3]);
		if(cArray[4][0]=='T')
			(*smTemp)->bIsInStock = true;
		else if(cArray[4][0]=='F')
			(*smTemp)->bIsInStock = false;
		(*smTemp)->iVolNo = atoi(cArray[5]);
	}
	else if(cArray[0][0]=='E')
	{
		(*smTemp)->eBookType = ENCYCLOPEDIA;
		strcpy((*smTemp)->cName,cArray[1]);
		strcpy((*smTemp)->cPublisher,cArray[2]);
		(*smTemp)->fPrice = atof(cArray[3]);	
		if(cArray[4][0]=='T')
			(*smTemp)->bIsInStock = true;
		else if(cArray[4][0]=='F')
			(*smTemp)->bIsInStock = false;
	}
}

void TrimWhiteSpace(char **cString)
{
	// leading whitespace
	while(isspace(**cString)) (*cString)++;

	// trailing whitespace
	char *end=NULL;
	end = *cString + strlen(*cString)-1;
	while(end > *cString && isspace(*end))
		end--;

	*(end+1)='\0';
}

void Usage()
{
	printf("\nUsage: bookstore -f [FILE]\n\n");
}

int FileNameLength(const char *cFileName)
{
	const char *pfile = cFileName;
	int len=0;

	while(*pfile)
	{
		len++;
		pfile++;
	}
	return len;
}

/*-------------------------------------------------------------------------*
 *---									---*
 *---		dirPath.c						---*
 *---									---*
 *---	    This file defines a program that parses a path into its	---*
 *---	component entries.						---*
 *---									---*
 *---	----	----	----	----	----	----	----	----	---*
 *---									---*
 *---	Version 1.0		2016 May 6		Joseph Phillips	---*
 *---									---*
 *-------------------------------------------------------------------------*/

#include	<stdlib.h>
#include	<stdio.h>
#include	<string.h>
#include	<ctype.h>

#define		LINE_LEN	256

struct	DirEntryName {
    char*			name_;
    struct DirEntryName*	nextPtr_;
};


struct	PathName {
    int			isRoot_;
    int			isMyHome_;
    int			isSomeonesHome_;
    struct DirEntryName*	dirEntryNamePtr_;
};


//  PURPOSE:  To return a non-zero value if 'c' is legal in a restricted Unix
//	directory entry name, or '0' otherwise.
int		isLegalDirEntryChar (char		c) {
    return( isalnum(c) || (c == '-') || (c == '_') || (c == '.') );
}


//  PURPOSE:  To return 'argv[1]' if there is at least one argument on the
//	command line according to 'argc'.  Otherwise:
//	(1) asks the user for a path,
//	(2) enters it into 'textSpace' of length 'textSpaceLen',
//	(3) removes the ending '\n' from 'textSpace',
//	(4) and returns 'textSpace'.
const char*	getPathText	(int		argc,
                         char*		argv[],
                         char* 		textSpace,
                         int		textSpaceLen
                         ) {
    if (argc >= 2) return argv[1];
    else {
        printf("Please designate your path: ");
        fgets(textSpace, textSpaceLen, stdin);
        char* cPtr = strchr(textSpace, '\n');
        if (cPtr != NULL) *cPtr = '\0';
        return textSpace;
    }
}

//  PURPOSE:  To create and return a linked list of heap-allocated
//	struct DirEntryName instances that represent the path 'linePtr'.
//	(1) If 'linePtr' is empty (points to '\0') then returns 'NULL'.
//	(2) If 'linePtr' is not empty then it
//	    (a) allocates a 'struct DirEntryName' instance from the heap,
//	    (b) allocates a C-string to hold the directory entry 'linePtr'
//	    	points to.
//	    (c) allocates a new 'struct DirEntryName' for the next directory
//	    	entry, etc.
//
//	Directory entries are separated by the '/' char.
//	If the directory entry is empty (e.g. "//") then it does:
//		fprintf(stderr,"Missing directory name!\n");
//		exit(EXIT_FAILURE);
//	If the directory entry has a character not accepted by
//	'isLegalDirEntryChar()' then it does:
//		fprintf(stderr,"Illegal character %c in path!\n",*linePtr);
//		exit(EXIT_FAILURE);

struct DirEntryName* parseRestOfPath	(const char*	linePtr) {
    //  I.  Application validity check:
    if  (linePtr == NULL) {
        fprintf(stderr,"NULL ptr to parseRestOfPath()\n");
        exit(EXIT_FAILURE);
    }
    
    //  II.  Return value:
    //  II.A.  Handle when at end of 'linePtr':
    if (*linePtr == '\0') return NULL;
    
    //  II.B.  Handle when 'linePtr' points to '/' and thus a missing directory name:
    if (*linePtr == '/') {
        fprintf(stderr,"Missing directory name!\n");
        exit(EXIT_FAILURE);
    }
    
    //  II.C.  Get entry name:
    const char*	entryNamePtr	= linePtr;
    
    //  II.C.1.  Leave 'entryNamePtr' pointing to the beginning of the entry name and advance 'linePtr' until the characters are no longer legal:
    int i;
    for (i = 0; isLegalDirEntryChar(linePtr[i]); i++) {  }
    linePtr += i;
    
    //  II.C.2.  If you have stopped because of anything other than '/' or '\0' then the user gave you an illegal char.  Give error message here:
    if (*linePtr != '\0' && *linePtr != '/') {
        fprintf(stderr,"Illegal character %c in path!\n",*linePtr);
        exit(EXIT_FAILURE);
    }
    
    //  II.C.3.  Allocate a new 'struct DirEntryName*' pointer here. Allocate memory for its name and copy entry name into that mem:
    int nameLen = linePtr - entryNamePtr;
    struct DirEntryName         *inst       = (struct DirEntryName *)malloc(sizeof(struct DirEntryName));
    char                        *name       = (char *)malloc(sizeof(char) * (nameLen + 1));
    memcpy(name, entryNamePtr, nameLen);
    name[nameLen] = '\0';
    inst->name_ = name;
    
    
    //  II.C.4.  If 'linePtr' encountered '/' it should get the value for 'nextPtr_' by recursion.
    //  If it points to '\0' it should set 'nextPtr_' to 'NULL'.
    if (*linePtr == '/') inst->nextPtr_ = parseRestOfPath(linePtr + 1);
    if (*linePtr == '\0') inst->nextPtr_ = NULL;
        
    //  III.  Finished:
    return inst;
}

//  PURPOSE:  To return the address of a heap-allocated 'struct PathName'
//	instance that encodes the path given by 'linePtr'.
struct PathName* getPath		(const char*	linePtr) {
    //  I.  Application validity check:
    if  (linePtr == NULL) {
        fprintf(stderr,"NULL ptr to getPath()!\n");
        exit(EXIT_FAILURE);
    }
    
    //  II.  Create 'struct PathName' object:
    //  II.A.  Obtain heap memory:
    struct PathName*	toReturn	= (struct PathName*)malloc(sizeof(struct PathName));
    
    //  II.B.  Initialize flags of '*toReturn':
    toReturn->isRoot_			= 0;
    toReturn->isMyHome_			= 0;
    toReturn->isSomeonesHome_	= 0;
    
    switch  (*linePtr) {
        case '/' :
            toReturn->isRoot_	= 1;
            linePtr++;
            break;
            
        case '~' :
            linePtr++;
            
            if  (*linePtr == '\0') toReturn->isMyHome_	= 1;
            else
                if  (*linePtr == '/') {
                    toReturn->isMyHome_	= 1;
                    linePtr++;
                } else toReturn->isSomeonesHome_	= 1;
            
            break;
        case '\0' :
            fprintf(stderr,"Empty path!\n");
            exit(EXIT_FAILURE);
    }
    
    //  II.B.  Initialize 'dirEntryNamePtr_' of '*toReturn':
    toReturn->dirEntryNamePtr_	= parseRestOfPath(linePtr);
    
    //  III.  Finished:
    return(toReturn);
}


//  PURPOSE:  To print out the constructed path '*pathNamePtr'.  No return
//	value.
void		print		(struct PathName*
                         pathNamePtr) {
    //  I.  Application validity check:
    if  (pathNamePtr == NULL) {
        fprintf(stderr,"NULL ptr to print()!\n");
        exit(EXIT_FAILURE);
    }
    
    //  II.  Print path:
    //  II.A.  Print beginning of path:
    int	sum	= pathNamePtr->isRoot_		+
		  pathNamePtr->isMyHome_	+
		  pathNamePtr->isSomeonesHome_;
    
    if  ( (sum < 0) || (sum > 1) ) {
        fprintf(stderr,"Inconsistent pathname!\n");
        exit(EXIT_FAILURE);
    }
    
    struct DirEntryName*	run	= pathNamePtr->dirEntryNamePtr_;
    
    if  (pathNamePtr->isRoot_) printf("Start from the root directory /\n");
    else
        if  (pathNamePtr->isMyHome_) printf("Start from your home directory ~\n");
        else
            if  (pathNamePtr->isSomeonesHome_) {
                printf("Start from the home directory of %s\n",run->name_);
                run		  = run->nextPtr_;
            } else printf("Start from current directory\n");
    
    //  II.B.  Print rest of path:
    for  ( ;  run != NULL;  run = run->nextPtr_) printf("  %s\n",run->name_);
    
    //  III.  Finished:
}


//  PURPOSE:  To 'free()' the memory of 'pathNamePtr': all DirEntryName
//	'name_' and 'nextPtr_' member vars, and the memory of 'pathNamePtr'
//	itself.  No return value.
void recursiveDestroy(struct DirEntryName *dir) {
    if (dir == NULL) return;
    else if (dir->nextPtr_ != NULL) recursiveDestroy(dir->nextPtr_);
    else {
        free(dir->name_);
        free(dir);
    }
}

void		destroy		(struct PathName* pathNamePtr) {
    struct DirEntryName *name = pathNamePtr->dirEntryNamePtr_;
    recursiveDestroy(name);
    free(pathNamePtr);
}


//  PURPOSE:  To do the high level work of this program.  'argc' tells the
//	number of command line arguments.  'argv[]' points to the arguments.
//	Returns 'EXIT_SUCCESS' to OS on success or 'EXIT_FAILURE' otherwise.
int		main		(int		argc, char*		argv[]) {
    char	textSpace[LINE_LEN];
    const char*		linePtr	= getPathText(argc,argv,textSpace,LINE_LEN);
    struct PathName*	pathPtr	= getPath(linePtr);
    
    print(pathPtr);
    destroy(pathPtr);
    return(EXIT_SUCCESS);
}
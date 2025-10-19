#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iso646.h>

/* Maximum stack limit. */
#define MAX 3
/* Length of the string 'name'. */
#define TAM 30

typedef struct person_t {
    int age;
    char name[ TAM ];
} person_t;

typedef struct node_t {
    person_t info;
    struct node_t* pNext;
} node_t;

typedef struct stack_t {
    node_t* pTop;
    node_t* pBase;
    int limit;
} stack_t;

typedef enum menu_t {
    MENU_PUSH     = 0,
    MENU_POP_TOP  = 1,
    MENU_POP_NAME = 2,
    MENU_CLEAR    = 3,
    MENU_LIST     = 4,
    MENU_EXIT     = 5
} menu_t;

short Menu( void );
void MenuSelect( menu_t* );

void ClearBuffer( void );
void ClearScreen( void );
void PressEnter( void );

int ValidateInteger( const char* );
void ReadPerson( person_t* );

void Reset( stack_t* );
bool Empty( const stack_t* );
bool Full( const stack_t* );
bool Push( stack_t*, const person_t* );
void Pop( stack_t*, node_t* );
void PopName( stack_t* );
void Clear( stack_t*, bool );
void List( stack_t* );

int main( void ) {
    const short status = Menu();

    return status;
}

/*!
 * @brief Main menu.
 * @retval 0 Success.
 * @retval 1 Failure.
 */
short Menu( void ) {
    ClearScreen();

    /* Main stack. */
    stack_t* stack = malloc( sizeof( stack_t ) );
    if ( !stack ) {
        printf( "Error - Could not dynamically allocate memory!" );
        return 1;
    }

    Reset( stack );

    menu_t menuOptions = -1;

    while ( menuOptions != MENU_EXIT ) {
        ClearScreen();
        MenuSelect( &menuOptions );
        ClearScreen();

        switch ( menuOptions ) {
            case MENU_PUSH:
                if ( Full( stack ) ) {
                    printf( "Error - Full Stack!\n" );
                    PressEnter();
                } else {
                    person_t person = {};
                    ReadPerson( &person );
                    Push( stack, &person );
                }
                break;

            case MENU_POP_TOP:
                if ( Empty( stack ) ) {
                    printf( "Error - Empty Stack!\n" );
                } else {
                    printf( "Age: %d\nName: %s\n", stack->pTop->info.age, stack->pTop->info.name );
                    Pop( stack, stack->pTop );
                }
                PressEnter();
                break;

            case MENU_POP_NAME:
                PopName( stack );
                break;

            case MENU_CLEAR:
                Clear( stack, false );
                break;

            case MENU_LIST:
                List( stack );
                break;

            case MENU_EXIT:
                Clear( stack, true );
                ClearScreen();
                break;

            default:
                printf( "Error - Invalid option! [%d]\n", menuOptions );
                PressEnter();
                break;
        }
    }

    free( stack );

    return 0;
}

/*!
 * @brief Select menu options.
 * @param [in] menuOptions Menu options.
 * @return Nothing.
 */
void MenuSelect( menu_t* menuOptions ) {
    char menuTemp[ 3 ];

    do {
        char* endptr;

        printf( ".:MENU:.\n\n" );
        printf( "%d - Insert Person\n", MENU_PUSH );
        printf( "%d - Delete Person from Top\n", MENU_POP_TOP );
        printf( "%d - Delete Person by Name\n", MENU_POP_NAME );
        printf( "%d - Clear the Stack\n", MENU_CLEAR );
        printf( "%d - List People on Screen\n", MENU_LIST );
        printf( "%d - Exit the Program\n\n", MENU_EXIT );
        printf( "Select the Option: " );

        fgets( menuTemp, sizeof( menuTemp ), stdin );
        menuTemp[ strcspn( menuTemp, "\n" ) ] = 0;
        if ( strlen( menuTemp ) >= 2 ) {
            ClearBuffer();
        }

        *menuOptions = (menu_t)strtol( menuTemp, &endptr, 10 );
        if ( *menuOptions < MENU_PUSH or *menuOptions > MENU_EXIT or menuTemp[ 0 ] < 48 or menuTemp[ 0 ] > 57 ) {
            ClearScreen();
            printf( "Error - Invalid option! [%s]\n", menuTemp );
            PressEnter();
            ClearScreen();
        }
    } while ( *menuOptions < MENU_PUSH or *menuOptions > MENU_EXIT or menuTemp[ 0 ] < 48 or menuTemp[ 0 ] > 57 );
}

/*!
 * @brief Clear keyboard buffer.
 * @return Nothing.
 */
void ClearBuffer( void ) {
    int buffer;
    while ( ( buffer = getchar() ) != '\n' and buffer != EOF );
}

/*!
 * @brief Clear prompt/terminal.
 * @return Nothing.
 */
void ClearScreen( void ) {
    printf( "\x1b[2J\x1b[H" );
}

/*!
 * @brief Press ENTER to continue.
 * @return Nothing.
 */
void PressEnter( void ) {
    printf( "\nPress ENTER..." );
    getchar();
}

/*!
 * @brief Convert string to integer.
 * @param [in] text Text to input string.
 * @return Integer converted from string.
 */
int ValidateInteger( const char* text ) {
    int value;
    char valueTemp[ 12 ];

    do {
        char* endptr;

        printf( "%s: ", text );

        fgets( valueTemp, sizeof( valueTemp ),stdin );
        valueTemp[ strcspn( valueTemp, "\n" ) ] = 0;
        if ( strlen( valueTemp ) >= 11 ) {
            ClearBuffer();
        }

        value = (int)strtol( valueTemp, &endptr, 10 );
        if ( value < 0 or value > 130 or valueTemp[ 0 ] < 48 or valueTemp[ 0 ] > 57 ) {
            ClearScreen();
            printf( "Error - Invalid value! [%s]\n", valueTemp );
            PressEnter();
            ClearScreen();
        }
    } while ( value < 0 or value > 130 or valueTemp[ 0 ] < 48 or valueTemp[ 0 ] > 57 );

    return value;
}

/*!
 * @brief Read 'age' and 'name'.
 * @param [in] person Person variable.
 * @return Nothing.
 */
void ReadPerson( person_t* person ) {
    person->age = ValidateInteger( "Enter Age" );

    printf( "Enter the Name: " );
    fgets( person->name, sizeof( person->name ),stdin );
    person->name[ strcspn( person->name, "\n" ) ] = 0;
}

/*!
 * @brief Reset stack.
 * @param [in] stack Stack variable.
 * @return Nothing.
 */
void Reset( stack_t* stack ) {
    stack->pTop = nullptr;
    stack->pBase = nullptr;
    stack->limit = 0;
}

/*!
 * @brief Checks if the stack is empty.
 * @param [in] stack Stack variable.
 * @retval True Empty stack.
 * @retval False Stack not empty.
 */
bool Empty( const stack_t* stack ) {
    return stack->pTop == nullptr;
}

/*!
 * @brief Checks if the stack is full.
 * @param [in] stack Stack variable.
 * @retval True Full stack.
 * @retval False Stack not full.
 */
bool Full( const stack_t* stack ) {
    return stack->limit >= MAX;
}

/*!
 * @brief Insert person into stack.
 * @param [in] stack Stack variable.
 * @param [in] person Person variable.
 * @retval True Success.
 * @retval False Failure.
 */
bool Push( stack_t* stack, const person_t* person ) {
    if ( Full( stack ) ) {
        printf( "Error - Full Stack!\n" );
        PressEnter();
        return false;
    }

    /* Stack node. */
    node_t* newNode = malloc( sizeof( node_t ) );
    if ( !newNode ) {
        printf( "Error - Could not dynamically allocate memory!" );
        return false;
    }

    newNode->info.age = person->age;
    strcpy( newNode->info.name, person->name );
    newNode->pNext = stack->pTop;

    stack->pTop = newNode;

    if ( stack->limit == 0 ) {
        stack->pBase = newNode;
    }

    stack->limit++;

    return true;
}

/*!
 * @brief Delete person from stack.
 * @param [in] stack Stack variable.
 * @param [in] node Node to delete.
 * @return Nothing.
 */
void Pop( stack_t* stack, node_t* node ) {
    if ( Empty( stack ) ) {
        printf( "Error - Empty Stack!\n" );
        PressEnter();
        return;
    }

    stack->limit--;

    if ( stack->limit == 0 ) {
        free( node );
        Reset( stack );
        return;
    }

    stack->pTop = node->pNext;
    node->pNext = nullptr;
    free( node );
}

/*!
 * @brief Delete person by name in stack.
 * @param [in] stack Stack variable.
 * @return Nothing.
 */
void PopName( stack_t* stack ) {
    if ( Empty( stack ) ) {
        printf( "Error - Empty Stack!\n" );
        PressEnter();
        return;
    }

    char name[ TAM ];

    printf( "Enter the Name to Delete: " );

    fgets( name, sizeof( name ), stdin );
    name[ strcspn( name, "\n" ) ] = 0;
    if ( strlen( name ) >= TAM - 1 ) {
        ClearBuffer();
    }

    ClearScreen();

    /* Secondary stack. */
    stack_t* newStack = malloc( sizeof( stack_t ) );
    if ( !newStack ) {
        printf( "Error - Could not dynamically allocate memory!" );
        return;
    }

    Reset( newStack );

    printf( ".:POP:.\n" );
    while ( !Empty( stack ) ) {
        if ( strcmp( name, stack->pTop->info.name ) != 0 ) {
            Push( newStack, &stack->pTop->info );
        }
        printf( "\nAge: %d\nName: %s\n", stack->pTop->info.age, stack->pTop->info.name );
        Pop( stack, stack->pTop );
    }
    PressEnter();
    ClearScreen();

    printf( ".:PUSH:.\n" );
    while ( !Empty( newStack ) ) {
        Push( stack, &newStack->pTop->info );
        printf( "\nAge: %d\nName: %s\n", newStack->pTop->info.age, newStack->pTop->info.name );
        Pop( newStack, newStack->pTop );
    }
    PressEnter();

    free( newStack );
}

/*!
 * @brief Clear stack.
 * @param [in] stack Stack variable.
 * @param [in] exit Control variable.
 * @return Nothing.
 */
void Clear( stack_t* stack, const bool exit ) {
    if ( Empty( stack ) ) {
        if ( !exit ) {
            printf( "Error - Empty Stack!\n" );
            PressEnter();
        }
    } else {
        while ( !Empty( stack ) ) {
            Pop( stack, stack->pTop );
        }
        if ( !exit ) {
            printf( "Clean Stack.\n" );
            PressEnter();
        }
    }
}

/*!
 * @brief List persons in the stack.
 * @param [in] stack Stack variable.
 * @return Nothing.
 */
void List( stack_t* stack ) {
    if ( Empty( stack ) ) {
        printf( "Error - Empty Stack!\n" );
        PressEnter();
        return;
    }

    /* Secondary stack. */
    stack_t* newStack = malloc( sizeof( stack_t ) );
    if ( !newStack ) {
        printf( "Error - Could not dynamically allocate memory!" );
        return;
    }

    Reset( newStack );

    printf( ".:POP:.\n" );
    while ( !Empty( stack ) ) {
        Push( newStack, &stack->pTop->info );
        printf( "\nAge: %d\nName: %s\n", stack->pTop->info.age, stack->pTop->info.name );
        Pop( stack, stack->pTop );
    }
    PressEnter();
    ClearScreen();

    printf( ".:PUSH:.\n" );
    while ( !Empty( newStack ) ) {
        Push( stack, &newStack->pTop->info );
        printf( "\nAge: %d\nName: %s\n", newStack->pTop->info.age, newStack->pTop->info.name );
        Pop( newStack, newStack->pTop );
    }
    PressEnter();

    free( newStack );
}

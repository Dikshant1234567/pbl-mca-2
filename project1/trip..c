#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_STRING 100
#define MAX_BOOKINGS 100
#define MAX_COMPANIES 5
#define HASH_SIZE 53  // Prime number for better distribution

// Hash table node structure
typedef struct HashNode {
    char key[MAX_STRING];
    void* data;
    struct HashNode* next;
} HashNode;

// Hash table structure
typedef struct {
    HashNode* table[HASH_SIZE];
} HashTable;

// Function to calculate hash value
unsigned int hash(const char* key) {
    unsigned int hash = 0;
    for (int i = 0; key[i] != '\0'; i++) {
        hash = hash * 31 + key[i];
    }
    return hash % HASH_SIZE;
}

// Initialize hash table
void initHashTable(HashTable* ht) {
    for (int i = 0; i < HASH_SIZE; i++) {
        ht->table[i] = NULL;
    }
}

// Insert into hash table
void insertHash(HashTable* ht, const char* key, void* data) {
    unsigned int index = hash(key);
    HashNode* newNode = (HashNode*)malloc(sizeof(HashNode));
    strcpy(newNode->key, key);
    newNode->data = data;
    newNode->next = ht->table[index];
    ht->table[index] = newNode;
}

// Search in hash table
void* searchHash(HashTable* ht, const char* key) {
    unsigned int index = hash(key);
    HashNode* current = ht->table[index];
    while (current != NULL) {
        if (strcmp(current->key, key) == 0) {
            return current->data;
        }
        current = current->next;
    }
    return NULL;
}

// Binary Search Tree for sorted bookings
typedef struct BookingNode {
    Booking booking;
    struct BookingNode* left;
    struct BookingNode* right;
} BookingNode;

// Insert booking into BST
BookingNode* insertBookingBST(BookingNode* root, Booking booking) {
    if (root == NULL) {
        BookingNode* newNode = (BookingNode*)malloc(sizeof(BookingNode));
        newNode->booking = booking;
        newNode->left = newNode->right = NULL;
        return newNode;
    }

    // Compare by booking date
    if (strcmp(booking.bookingDate, root->booking.bookingDate) < 0) {
        root->left = insertBookingBST(root->left, booking);
    } else {
        root->right = insertBookingBST(root->right, booking);
    }
    return root;
}

// Inorder traversal of BST
void inorderTraversal(BookingNode* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        // Display booking
        if (root->booking.isTourCompany) {
            printf("Tour: %s [%s]\n", 
                   root->booking.name,
                   root->booking.countryName);
            printf("   Duration: %d days\n", root->booking.tripDays);
            printf("   Accommodation: %s\n", root->booking.accommodation);
        } else {
            printf("Hotel: %s [%s] (Rating: %.1f)\n", 
                   root->booking.name,
                   root->booking.countryName,
                   root->booking.rating);
        }
        printf("   Booked by: %s\n", root->booking.userName);
        printf("   Booking Date: %s\n", root->booking.bookingDate);
        printf("   Budget: $%.2f\n", root->booking.budget);
        inorderTraversal(root->right);
    }
}

// Structure for tour company information
typedef struct {
    char name[MAX_STRING];
    float budget;
    int tripDays;
    char accommodation[MAX_STRING];
    char country[MAX_STRING];
} TourCompany;

// Structure for hotel information
typedef struct {
    char name[MAX_STRING];
    float rating;
    float price;
} Hotel;

// Structure for place information
typedef struct {
    char name[MAX_STRING];
    Hotel hotels[10];
    int hotelCount;
} Place;

// Structure for country information
typedef struct {
    char name[MAX_STRING];
    float minBudget;
    Place places[10];
    int placeCount;
} Country;

// Structure for booking information
typedef struct {
    char name[MAX_STRING];  // Hotel name or company name
    char countryName[MAX_STRING];
    float rating;
    char userName[MAX_STRING];
    char bookingDate[MAX_STRING];
    float budget;
    int isTourCompany;  // Flag to distinguish between hotel and tour company bookings
    int tripDays;      // For tour company bookings
    char accommodation[MAX_STRING];  // For tour company bookings
} Booking;

// Queue structure for bookings
typedef struct {
    Booking bookings[MAX_BOOKINGS];
    int front;
    int rear;
    int size;
} BookingQueue;

// Function prototypes
void initializeQueue(BookingQueue* queue);
void enqueue(BookingQueue* queue, Booking booking);
void displayBookings(BookingQueue* queue);
void displayCountries(Country countries[], int count);
void displayPlaces(Country* country);
void displayHotels(Place* place);
int isValidFutureDate(const char* date);
void bookTourCompany(BookingQueue* queue, TourCompany* company);
void bookHotel(BookingQueue* queue, char* hotelName, char* countryName, float rating, float minBudget);
void mainMenu(Country countries[], int countryCount, BookingQueue* queue, TourCompany companies[], int companyCount);
void displayTourCompanies(TourCompany companies[], int count);
void displayCompanyDetails(TourCompany* company);

// Initialize the booking queue
void initializeQueue(BookingQueue* queue) {
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
}

// Add booking to queue
void enqueue(BookingQueue* queue, Booking booking) {
    if (queue->size < MAX_BOOKINGS) {
        queue->rear = (queue->rear + 1) % MAX_BOOKINGS;
        queue->bookings[queue->rear] = booking;
        queue->size++;
    }
}

// Display all bookings
void displayBookings(BookingQueue* queue) {
    if (queue->size == 0) {
        printf("\nNo bookings found!\n");
        return;
    }

    printf("\n=== My Bookings (Sorted by Date) ===\n");
    BookingNode* bookingBST = NULL;
    
    // Create BST from queue
    int i = queue->front;
    int count = 0;
    while (count < queue->size) {
        bookingBST = insertBookingBST(bookingBST, queue->bookings[i]);
        i = (i + 1) % MAX_BOOKINGS;
        count++;
    }

    // Display sorted bookings
    inorderTraversal(bookingBST);
}

// Display available countries
void displayCountries(Country countries[], int count) {
    printf("\n=== Available Countries ===\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s [Budget: $%.2f]\n", i + 1, countries[i].name, countries[i].minBudget);
    }
}

// Display places in a country
void displayPlaces(Country* country) {
    printf("\n=== Famous Places in %s ===\n", country->name);
    for (int i = 0; i < country->placeCount; i++) {
        printf("%d. %s\n", i + 1, country->places[i].name);
    }
}

// Display hotels in a place
void displayHotels(Place* place) {
    printf("\n=== Hotels in %s ===\n", place->name);
    for (int i = 0; i < place->hotelCount; i++) {
        printf("%d. %s (Rating: %.1f, Price: $%.2f)\n", 
               i + 1, 
               place->hotels[i].name, 
               place->hotels[i].rating,
               place->hotels[i].price);
    }
}

// Display tour companies
void displayTourCompanies(TourCompany companies[], int count) {
    printf("\n=== Tour Companies ===\n");
    for (int i = 0; i < count; i++) {
        printf("%d. %s [Budget: $%.2f] - %s Tour\n", 
               i + 1, 
               companies[i].name, 
               companies[i].budget,
               companies[i].country);
    }
}

// Display company details
void displayCompanyDetails(TourCompany* company) {
    printf("\n=== %s Tour Details ===\n", company->name);
    printf("Country: %s\n", company->country);
    printf("Budget: $%.2f\n", company->budget);
    printf("Trip Duration: %d days\n", company->tripDays);
    printf("Accommodation: %s\n", company->accommodation);
}

// Validate if the date is in the future
int isValidFutureDate(const char* date) {
    int day, month, year;
    char separator;
    
    // Try parsing with forward slash
    if (sscanf(date, "%d/%d/%d", &day, &month, &year) == 3) {
        separator = '/';
    }
    // Try parsing with hyphen
    else if (sscanf(date, "%d-%d-%d", &day, &month, &year) == 3) {
        separator = '-';
    }
    else {
        printf("Error: Invalid date format. Please use DD/MM/YYYY or DD-MM-YYYY\n");
        return 0;
    }

    // Get current date
    time_t t = time(NULL);
    struct tm* current = localtime(&t);
    int currentYear = current->tm_year + 1900;
    int currentMonth = current->tm_mon + 1;
    int currentDay = current->tm_mday;

    // Print debug information
    printf("Debug - Current date: %d/%d/%d\n", currentDay, currentMonth, currentYear);
    printf("Debug - Input date: %d%c%d%c%d\n", day, separator, month, separator, year);

    // Basic date validation
    if (year < currentYear || year > currentYear + 5) {
        printf("Error: Year must be between %d and %d\n", currentYear, currentYear + 5);
        return 0;
    }
    if (month < 1 || month > 12) {
        printf("Error: Month must be between 1 and 12\n");
        return 0;
    }
    if (day < 1 || day > 31) {
        printf("Error: Day must be between 1 and 31\n");
        return 0;
    }

    // Check if date is in the future
    if (year < currentYear) {
        printf("Error: Year must be in the future\n");
        return 0;
    }
    if (year == currentYear && month < currentMonth) {
        printf("Error: Month must be in the future\n");
        return 0;
    }
    if (year == currentYear && month == currentMonth && day <= currentDay) {
        printf("Error: Day must be in the future\n");
        return 0;
    }

    // Additional validation for months with 30 days
    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) {
        printf("Error: This month has only 30 days\n");
        return 0;
    }
    
    // February validation
    if (month == 2) {
        int isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
        if (isLeapYear && day > 29) {
            printf("Error: February has only 29 days in leap year\n");
            return 0;
        }
        if (!isLeapYear && day > 28) {
            printf("Error: February has only 28 days in non-leap year\n");
            return 0;
        }
    }

    return 1;
}

// Book a tour company
void bookTourCompany(BookingQueue* queue, TourCompany* company) {
    Booking newBooking;
    strcpy(newBooking.name, company->name);
    strcpy(newBooking.countryName, company->country);
    newBooking.rating = 0.0;  // Tour companies don't have ratings
    newBooking.isTourCompany = 1;
    newBooking.tripDays = company->tripDays;
    strcpy(newBooking.accommodation, company->accommodation);

    // Get user name
    printf("\nEnter your name: ");
    fgets(newBooking.userName, MAX_STRING, stdin);
    newBooking.userName[strcspn(newBooking.userName, "\n")] = 0;  // Remove newline

    // Get user budget
    printf("Enter your budget (in USD): $");
    scanf("%f", &newBooking.budget);
    getchar(); // Clear input buffer

    // Check if budget is sufficient
    if (newBooking.budget < company->budget) {
        printf("\nError: Your budget ($%.2f) is below the required budget ($%.2f) for this tour.\n", 
               newBooking.budget, company->budget);
        return;
    }

    // Get current date
    time_t t = time(NULL);
    struct tm* current = localtime(&t);
    sprintf(newBooking.bookingDate, "%02d/%02d/%d", 
            current->tm_mday, 
            current->tm_mon + 1, 
            current->tm_year + 1900);

    // Validate inputs
    if (strlen(newBooking.userName) == 0) {
        printf("\nError: Name is required for booking!\n");
        return;
    }

    enqueue(queue, newBooking);
    printf("\nCongratulations! Your tour booking has been confirmed in %s.\n", newBooking.bookingDate);
}

// Book a hotel
void bookHotel(BookingQueue* queue, char* hotelName, char* countryName, float rating, float minBudget) {
    Booking newBooking;
    strcpy(newBooking.name, hotelName);
    strcpy(newBooking.countryName, countryName);
    newBooking.rating = rating;
    newBooking.isTourCompany = 0;

    // Get user name
    printf("\nEnter your name: ");
    fgets(newBooking.userName, MAX_STRING, stdin);
    newBooking.userName[strcspn(newBooking.userName, "\n")] = 0;  // Remove newline

    // Get user budget
    printf("Enter your budget (in USD): $");
    scanf("%f", &newBooking.budget);
    getchar(); // Clear input buffer

    // Check if budget is sufficient
    if (newBooking.budget < minBudget) {
        printf("\nError: Your budget ($%.2f) is below the minimum required budget ($%.2f) for this country.\n", 
               newBooking.budget, minBudget);
        return;
    }

    // Get and validate booking date
    int validDate = 0;
    do {
        printf("Enter booking date (DD/MM/YYYY or DD-MM-YYYY): ");
        fgets(newBooking.bookingDate, MAX_STRING, stdin);
        newBooking.bookingDate[strcspn(newBooking.bookingDate, "\n")] = 0;  // Remove newline

        if (!isValidFutureDate(newBooking.bookingDate)) {
            printf("Please try again with a valid future date.\n");
        } else {
            validDate = 1;
        }
    } while (!validDate);

    // Validate inputs
    if (strlen(newBooking.userName) == 0) {
        printf("\nError: Name is required for booking!\n");
        return;
    }

    enqueue(queue, newBooking);
    printf("\nCongratulations! Your hotel booking has been confirmed.\n");
}

// Main menu function
void mainMenu(Country countries[], int countryCount, BookingQueue* queue, TourCompany companies[], int companyCount) {
    int choice;
    while (1) {
        printf("\n=== Travel Planner ===\n");
        printf("1. Select Country\n");
        printf("2. View My Bookings\n");
        printf("3. See More (Tour Companies)\n");
        printf("4. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // Clear input buffer

        switch (choice) {
            case 1: {
                displayCountries(countries, countryCount);
                int countryChoice;
                printf("Select a country (1-%d): ", countryCount);
                scanf("%d", &countryChoice);
                getchar();

                if (countryChoice > 0 && countryChoice <= countryCount) {
                    Country* selectedCountry = &countries[countryChoice - 1];
                    displayPlaces(selectedCountry);
                    
                    int placeChoice;
                    printf("Select a place (1-%d): ", selectedCountry->placeCount);
                    scanf("%d", &placeChoice);
                    getchar();

                    if (placeChoice > 0 && placeChoice <= selectedCountry->placeCount) {
                        Place* selectedPlace = &selectedCountry->places[placeChoice - 1];
                        displayHotels(selectedPlace);

                        int hotelChoice;
                        printf("Select a hotel (1-%d): ", selectedPlace->hotelCount);
                        scanf("%d", &hotelChoice);
                        getchar();

                        if (hotelChoice > 0 && hotelChoice <= selectedPlace->hotelCount) {
                            Hotel* selectedHotel = &selectedPlace->hotels[hotelChoice - 1];
                            printf("\nDo you want to book %s? (1 for Yes, 0 for No): ", selectedHotel->name);
                            int bookChoice;
                            scanf("%d", &bookChoice);
                            getchar();

                            if (bookChoice == 1) {
                                bookHotel(queue, selectedHotel->name, selectedCountry->name, 
                                        selectedHotel->rating, selectedCountry->minBudget);
                            }
                        }
                    }
                }
                break;
            }
            case 2:
                displayBookings(queue);
                break;
            case 3: {
                printf("\n=== Tour Company Information ===\n");
                printf("1. View Tour Companies\n");
                printf("2. Back to Main Menu\n");
                printf("Enter your choice: ");
                int tourChoice;
                scanf("%d", &tourChoice);
                getchar();

                if (tourChoice == 1) {
                    displayTourCompanies(companies, companyCount);
                    printf("\nSelect a company to view details (1-%d): ", companyCount);
                    int companyChoice;
                    scanf("%d", &companyChoice);
                    getchar();

                    if (companyChoice > 0 && companyChoice <= companyCount) {
                        displayCompanyDetails(&companies[companyChoice - 1]);
                        printf("\nDo you want to book this tour? (1 for Yes, 0 for No): ");
                        int bookChoice;
                        scanf("%d", &bookChoice);
                        getchar();

                        if (bookChoice == 1) {
                            bookTourCompany(queue, &companies[companyChoice - 1]);
                        }
                    }
                }
                break;
            }
            case 4:
                printf("\nThank you for using Travel Planner!\n");
                exit(0);
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
    }
}

// Modified search functions to use hash tables
Country* findCountry(HashTable* ht, const char* countryName) {
    return (Country*)searchHash(ht, countryName);
}

TourCompany* findCompany(HashTable* ht, const char* companyName) {
    return (TourCompany*)searchHash(ht, companyName);
}

int main() {
    // Initialize hash tables
    HashTable countryHash;
    HashTable companyHash;
    initHashTable(&countryHash);
    initHashTable(&companyHash);

    // Initialize sample data
    Country countries[] = {
        {
            "Japan",
            2000.0,  // Minimum budget in USD
            {
                {
                    "Tokyo",
                    {
                        {"Tokyo Grand Hotel", 4.5, 300.0},
                        {"Sakura Inn", 4.2, 250.0},
                        {"Imperial Palace Hotel", 4.8, 400.0}
                    },
                    3
                },
                {
                    "Kyoto",
                    {
                        {"Kyoto Traditional Inn", 4.6, 280.0},
                        {"Bamboo Garden Hotel", 4.3, 220.0},
                        {"Temple View Resort", 4.7, 350.0}
                    },
                    3
                }
            },
            2
        },
        {
            "France",
            1500.0,  // Minimum budget in USD
            {
                {
                    "Paris",
                    {
                        {"Eiffel Tower Hotel", 4.7, 350.0},
                        {"Louvre Palace", 4.5, 300.0},
                        {"Seine River View", 4.4, 280.0}
                    },
                    3
                },
                {
                    "Nice",
                    {
                        {"Mediterranean Resort", 4.6, 320.0},
                        {"French Riviera Hotel", 4.3, 250.0},
                        {"Coastal View Inn", 4.5, 290.0}
                    },
                    3
                }
            },
            2
        }
    };

    // Initialize tour companies
    TourCompany companies[] = {
        {
            "Sakura Tours",
            2500.0,
            7,
            "4-Star Hotels with Breakfast",
            "Japan"
        },
        {
            "Paris Explorer",
            2000.0,
            5,
            "3-Star Hotels with Half Board",
            "France"
        },
        {
            "Tokyo Adventures",
            3000.0,
            10,
            "5-Star Hotels with All-Inclusive",
            "Japan"
        },
        {
            "French Riviera Tours",
            2200.0,
            6,
            "4-Star Hotels with Breakfast",
            "France"
        }
    };

    // Insert countries and companies into hash tables
    for (int i = 0; i < 2; i++) {
        insertHash(&countryHash, countries[i].name, &countries[i]);
    }
    for (int i = 0; i < 4; i++) {
        insertHash(&companyHash, companies[i].name, &companies[i]);
    }

    // Initialize booking queue and BST
    BookingQueue bookingQueue;
    initializeQueue(&bookingQueue);
    BookingNode* bookingBST = NULL;

    mainMenu(countries, 2, &bookingQueue, companies, 4);

    return 0;
} 
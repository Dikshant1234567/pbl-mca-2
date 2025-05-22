#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure for hotel information
typedef struct Hotel {
    char name[100];
    float rating;
    struct Hotel* next;
} Hotel;

// Structure for place information
typedef struct Place {
    char name[100];
    Hotel* hotels;
    struct Place* next;
} Place;

// Structure for country information
typedef struct Country {
    char name[50];
    Place* places;
    struct Country* next;
} Country;

// Structure for booking information
typedef struct Booking {
    char hotelName[100];
    char countryName[50];
    float rating;
    struct Booking* next;
} Booking;

// Queue structure for bookings
typedef struct {
    Booking* front;
    Booking* rear;
} BookingQueue;

// Initialize booking queue
void initQueue(BookingQueue* q) {
    q->front = q->rear = NULL;
}

// Add booking to queue
void enqueue(BookingQueue* q, const char* hotelName, const char* countryName, float rating) {
    Booking* newBooking = (Booking*)malloc(sizeof(Booking));
    strcpy(newBooking->hotelName, hotelName);
    strcpy(newBooking->countryName, countryName);
    newBooking->rating = rating;
    newBooking->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = newBooking;
    } else {
        q->rear->next = newBooking;
        q->rear = newBooking;
    }
}

// Display bookings from queue
void displayBookings(BookingQueue* q) {
    Booking* current = q->front;
    if (!current) {
        printf("\nNo bookings found!\n");
        return;
    }

    printf("\n=== Your Bookings ===\n");
    int count = 1;
    while (current != NULL) {
        printf("%d. %s [%s] (Rating: %.1f)\n", 
              count++, current->hotelName, current->countryName, current->rating);
        current = current->next;
    }
}

// Free booking queue memory
void freeQueue(BookingQueue* q) {
    while (q->front != NULL) {
        Booking* temp = q->front;
        q->front = q->front->next;
        free(temp);
    }
    q->rear = NULL;
}

// Sample data initialization
Country* initializeCountries() {
    // France data
    Country* france = (Country*)malloc(sizeof(Country));
    strcpy(france->name, "France");
    
    Place* eiffel = (Place*)malloc(sizeof(Place));
    strcpy(eiffel->name, "Eiffel Tower");
    eiffel->hotels = (Hotel*)malloc(sizeof(Hotel));
    strcpy(eiffel->hotels->name, "Hotel de Londres Eiffel");
    eiffel->hotels->rating = 4.6;
    eiffel->hotels->next = (Hotel*)malloc(sizeof(Hotel));
    strcpy(eiffel->hotels->next->name, "Hôtel Le Derby Alma");
    eiffel->hotels->next->rating = 4.5;
    eiffel->hotels->next->next = NULL;
    
    france->places = eiffel;

    // USA data
    Country* usa = (Country*)malloc(sizeof(Country));
    strcpy(usa->name, "USA");
    
    Place* statue = (Place*)malloc(sizeof(Place));
    strcpy(statue->name, "Statue of Liberty");
    statue->hotels = (Hotel*)malloc(sizeof(Hotel));
    strcpy(statue->hotels->name, "Hilton Garden Inn NYC");
    statue->hotels->rating = 4.5;
    statue->hotels->next = NULL;
    
    usa->places = statue;

    // Link countries
    france->next = usa;
    usa->next = NULL;

    return france;
}

// Main application logic
void travelPlanner() {
    Country* countries = initializeCountries();
    BookingQueue bookings;
    initQueue(&bookings);
    
    int mainChoice, countryChoice, placeChoice, hotelChoice;
    char bookChoice;
    Country* currentCountry;
    Place* currentPlace;
    Hotel* currentHotel;

    printf("=== Travel Planner ===\n");

    while(1) {
        printf("\nMain Menu:\n");
        printf("1. Select Country\n");
        printf("2. My Bookings\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &mainChoice);

        if(mainChoice == 3) break;

        if(mainChoice == 2) {
            displayBookings(&bookings);
            printf("\nContinue using application? (y/n): ");
            scanf(" %c", &bookChoice);
            if(bookChoice == 'n' || bookChoice == 'N') break;
            continue;
        }

        // Country selection
        printf("\nAvailable Countries:\n");
        Country* temp = countries;
        int count = 1;
        while(temp != NULL) {
            printf("%d. %s\n", count++, temp->name);
            temp = temp->next;
        }

        printf("Select country: ");
        scanf("%d", &countryChoice);

        // Navigate to selected country
        currentCountry = countries;
        for(int i = 1; i < countryChoice; i++) {
            if(currentCountry) currentCountry = currentCountry->next;
        }

        if(!currentCountry) {
            printf("Invalid selection!\n");
            continue;
        }

        // Place selection
        printf("\nFamous Places in %s:\n", currentCountry->name);
        currentPlace = currentCountry->places;
        count = 1;
        while(currentPlace != NULL) {
            printf("%d. %s\n", count++, currentPlace->name);
            currentPlace = currentPlace->next;
        }

        printf("Select place: ");
        scanf("%d", &placeChoice);

        // Navigate to selected place
        currentPlace = currentCountry->places;
        for(int i = 1; i < placeChoice; i++) {
            if(currentPlace) currentPlace = currentPlace->next;
        }

        if(!currentPlace) {
            printf("Invalid selection!\n");
            continue;
        }

        // Hotel selection
        printf("\nHotels near %s:\n", currentPlace->name);
        currentHotel = currentPlace->hotels;
        count = 1;
        while(currentHotel != NULL) {
            printf("%d. %s (Rating: %.1f)\n", count++, currentHotel->name, currentHotel->rating);
            currentHotel = currentHotel->next;
        }

        printf("Select hotel: ");
        scanf("%d", &hotelChoice);

        // Navigate to selected hotel
        currentHotel = currentPlace->hotels;
        for(int i = 1; i < hotelChoice; i++) {
            if(currentHotel) currentHotel = currentHotel->next;
        }

        if(!currentHotel) {
            printf("Invalid selection!\n");
            continue;
        }

        // Booking confirmation
        printf("\nBook %s? (y/n): ", currentHotel->name);
        scanf(" %c", &bookChoice);

        if(bookChoice == 'y' || bookChoice == 'Y') {
            enqueue(&bookings, currentHotel->name, currentCountry->name, currentHotel->rating);
            printf("\nCongratulations! Booking confirmed.\n");

            // Post-booking menu
            while(1) {
                printf("\n1. My Bookings\n2. Select Country\n3. Exit\nChoice: ");
                scanf("%d", &mainChoice);

                if(mainChoice == 1) displayBookings(&bookings);
                if(mainChoice == 2) break;
                if(mainChoice == 3) {
                    freeQueue(&bookings);
                    return;
                }
            }
        }
    }

    // Cleanup
    freeQueue(&bookings);
}

int main() {
    travelPlanner();
    return 0;
}

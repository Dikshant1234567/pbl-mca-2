#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Hotel linked list node
typedef struct HotelNode {
    char name[100];
    float rating;
    float distance;
    struct HotelNode* next;
} HotelNode;

// Place linked list node
typedef struct PlaceNode {
    char name[100];
    HotelNode* hotels;
    struct PlaceNode* next;
} PlaceNode;

// Country struct with linked list of places
typedef struct CountryNode {
    char name[50];
    PlaceNode* places;
    struct CountryNode* next;
} CountryNode;

// Simulated API data structure
typedef struct {
    char name[100];
    float rating;
    float distance;
} APIHotel;

typedef struct {
    char name[100];
    APIHotel* hotels;
    int hotel_count;
} APIPlace;

typedef struct {
    char name[50];
    APIPlace* places;
    int place_count;
} APICountry;

// Function to create hotel node
HotelNode* create_hotel_node(const char* name, float rating, float distance) {
    HotelNode* new_node = (HotelNode*)malloc(sizeof(HotelNode));
    strcpy(new_node->name, name);
    new_node->rating = rating;
    new_node->distance = distance;
    new_node->next = NULL;
    return new_node;
}

// Function to create place node
PlaceNode* create_place_node(const char* name) {
    PlaceNode* new_node = (PlaceNode*)malloc(sizeof(PlaceNode));
    strcpy(new_node->name, name);
    new_node->hotels = NULL;
    new_node->next = NULL;
    return new_node;
}

// Function to create country node
CountryNode* create_country_node(const char* name) {
    CountryNode* new_node = (CountryNode*)malloc(sizeof(CountryNode));
    strcpy(new_node->name, name);
    new_node->places = NULL;
    new_node->next = NULL;
    return new_node;
}

// Convert API data to linked lists
CountryNode* api_to_linked_list(APICountry* api_data, int country_count) {
    CountryNode* countries_head = NULL;
    CountryNode* current_country = NULL;

    for (int i = 0; i < country_count; i++) {
        CountryNode* country_node = create_country_node(api_data[i].name);
        
        PlaceNode* places_head = NULL;
        PlaceNode* current_place = NULL;
        
        for (int j = 0; j < api_data[i].place_count; j++) {
            APIPlace api_place = api_data[i].places[j];
            PlaceNode* place_node = create_place_node(api_place.name);
            
            HotelNode* hotels_head = NULL;
            HotelNode* current_hotel = NULL;
            
            for (int k = 0; k < api_place.hotel_count; k++) {
                APIHotel api_hotel = api_place.hotels[k];
                HotelNode* hotel_node = create_hotel_node(
                    api_hotel.name, api_hotel.rating, api_hotel.distance);
                
                if (!hotels_head) {
                    hotels_head = hotel_node;
                    current_hotel = hotels_head;
                } else {
                    current_hotel->next = hotel_node;
                    current_hotel = current_hotel->next;
                }
            }
            
            place_node->hotels = hotels_head;
            
            if (!places_head) {
                places_head = place_node;
                current_place = places_head;
            } else {
                current_place->next = place_node;
                current_place = current_place->next;
            }
        }
        
        country_node->places = places_head;
        
        if (!countries_head) {
            countries_head = country_node;
            current_country = countries_head;
        } else {
            current_country->next = country_node;
            current_country = current_country->next;
        }
    }
    
    return countries_head;
}

// Simulated API data
APICountry mock_api[] = {
    { // France
        "France",
        (APIPlace[]){
            {"Eiffel Tower", 
             (APIHotel[]){
                 {"Hotel de Londres Eiffel", 4.6, 0.5},
                 {"Hôtel Le Derby Alma", 4.5, 0.5}
             }, 2},
            {"Louvre Museum", 
             (APIHotel[]){
                 {"Hôtel Regina Louvre", 4.8, 0.2},
                 {"Grand Hotel Du Palais Royal", 4.7, 0.3}
             }, 2}
        }, 2
    },
    { // USA
        "USA",
        (APIPlace[]){
            {"Statue of Liberty", 
             (APIHotel[]){
                 {"Hilton Garden Inn NYC", 4.5, 3.2},
                 {"Canopy by Hilton", 4.6, 3.4}
             }, 2}
        }, 1
    }
};

int main() {
    // Convert API data to linked lists
    int country_count = sizeof(mock_api)/sizeof(mock_api[0]);
    CountryNode* countries = api_to_linked_list(mock_api, country_count);

    // Application loop (similar to previous version)
    CountryNode* current_country = countries;
    while(current_country) {
        printf("Country: %s\n", current_country->name);
        PlaceNode* current_place = current_country->places;
        while(current_place) {
            printf("  Place: %s\n", current_place->name);
            HotelNode* current_hotel = current_place->hotels;
            while(current_hotel) {
                printf("    Hotel: %s (%.1f stars, %.1f km)\n", 
                      current_hotel->name, current_hotel->rating, current_hotel->distance);
                current_hotel = current_hotel->next;
            }
            current_place = current_place->next;
        }
        current_country = current_country->next;
    }

    // Free memory (implementation omitted for brevity)
    return 0;
}

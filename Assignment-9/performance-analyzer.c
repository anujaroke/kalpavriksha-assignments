#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Players_data.h"

#define MAX_TEAMS 10
#define MAX_NAME 50
#define MAX_TEAM_PLAYERS 50

typedef struct player {
  int player_id;
  char name[MAX_NAME + 1];
  char team_name[MAX_NAME + 1];
  int role;
  int total_runs;
  float batting_average;
  float strike_rate;
  int wickets;
  float economy_rate;
  float performance_index;
  struct player *next;
  struct player *next_in_role;
} player;

typedef struct team {
  int team_id;
  char name[MAX_NAME + 1];
  int total_players;
  int batter_count;
  float strike_rate_sum;
  float average_strike_rate;
  player *head;
  player *tail;
  player *role_head[3];
} team;

const char *role_names[] = {"", "Batsman", "Bowler", "All-rounder"};
team team_list[MAX_TEAMS];
int team_count;


int read_int(const char message[], int *value) {

  printf("%s", message);
  return scanf("%d", value) == 1;

}

int read_float(const char message[], float *value) {

  printf("%s", message);
  return scanf("%f", value) == 1;

}

int role_from_text(const char text[]) {

  if (strcmp(text, "Batsman") == 0) {
    return 1;
  }

  if (strcmp(text, "Bowler") == 0) {
    return 2;
  }

  return 3;

}

team *find_team(int team_id) {

  int low = 0;
  int high = team_count - 1;
  int middle;

  while (low <= high) {

    middle = (low + high) / 2;
    if (team_list[middle].team_id == team_id) {
      return &team_list[middle];
    }

    if (team_list[middle].team_id < team_id) {
      low = middle + 1;
    }

    else {
      high = middle - 1;
    }

  }

  return NULL;

}

team *find_team_by_name(const char name[]) {

  int index;

  for (index = 0; index < team_count; index++) {
    if (strcmp(team_list[index].name, name) == 0) {
      return &team_list[index];
    }
  }

  return NULL;

}

int player_id_exists(int player_id) {

  int index;
  player *current;

  for (index = 0; index < team_count; index++) {

    current = team_list[index].head;
    while (current != NULL) {
      if (current->player_id == player_id) {
        return 1;
      }
      current = current->next;
      
    }

  }

  return 0;

}

player *create_player(int player_id, const char name[], const char team_name[], int role, int total_runs, float batting_average, float strike_rate, int wickets, float economy_rate) {

  player *new_player = calloc(1, sizeof(player));

  if (new_player == NULL) {
    return NULL;
  }

  new_player->player_id = player_id;
  snprintf(new_player->name, sizeof(new_player->name), "%s", name);
  snprintf(new_player->team_name, sizeof(new_player->team_name), "%s", team_name);
  new_player->role = role;
  new_player->total_runs = total_runs;
  new_player->batting_average = batting_average;
  new_player->strike_rate = strike_rate;
  new_player->wickets = wickets;
  new_player->economy_rate = economy_rate;

  switch (role) {
    case 1:
      new_player->performance_index = batting_average * strike_rate / 100;
      break;

    case 2:
      new_player->performance_index = wickets * 2 + (100 - economy_rate);
      break;

    default:
      new_player->performance_index = batting_average * strike_rate / 100 + wickets * 2;
  }

  return new_player;

}

void add_player_to_team(team *selected_team, player *new_player) {

  player *previous = NULL;
  player *current = selected_team->role_head[new_player->role - 1];

  if (selected_team->tail == NULL) {
    selected_team->head = new_player;
  }

  else {
    selected_team->tail->next = new_player;
  }

  selected_team->tail = new_player;

  while (current != NULL && current->performance_index >= new_player->performance_index) {
    previous = current;
    current = current->next_in_role;
  }

  new_player->next_in_role = current;

  if (previous == NULL) {
    selected_team->role_head[new_player->role - 1] = new_player;
  }

  else {
    previous->next_in_role = new_player;
  }

  selected_team->total_players++;

  if (new_player->role != 2) {
    selected_team->strike_rate_sum += new_player->strike_rate;
    selected_team->batter_count++;
    selected_team->average_strike_rate = selected_team->strike_rate_sum / selected_team->batter_count;
  }

}

void print_table_header(int show_team) {

  int width = 84;
  int index;

  if (show_team) {
    width = 99;
  }

  for (index = 0; index < width; index++) {
    printf("=");
  }

  printf("\n%-5s %-24s ", "ID", "Name");

  if (show_team) {
    printf("%-14s ", "Team");
  }

  printf("%-12s %6s %5s %6s %5s %4s %9s\n", "Role", "Runs", "Avg", "SR", "Wkts", "ER", "Perf.Index");

  for (index = 0; index < width; index++) {
    printf("=");
  }

  printf("\n");

}

void print_player_row(player *selected_player, int show_team) {

  printf("%-5d %-24s ", selected_player->player_id, selected_player->name);

  if (show_team) {
    printf("%-14s ", selected_player->team_name);
  }

  printf("%-12s %6d %5.1f %6.1f %5d %4.1f %9.2f\n", role_names[selected_player->role], selected_player->total_runs, selected_player->batting_average, selected_player->strike_rate, selected_player->wickets, selected_player->economy_rate, selected_player->performance_index);

}

void add_player() {

  int team_id;
  int player_id;
  int role;
  int total_runs;
  int wickets;
  float batting_average;
  float strike_rate;
  float economy_rate;
  char name[101];
  team *selected_team;
  player *new_player;

  if (!read_int("Enter Team ID to add player: ", &team_id)) {
    printf("Invalid team ID.\n");
    return;
  }

  selected_team = find_team(team_id);

  if (selected_team == NULL) {
    printf("Team not found.\n");
    return;
  }

  if (selected_team->total_players >= MAX_TEAM_PLAYERS) {
    printf("Team already has %d players.\n", MAX_TEAM_PLAYERS);
    return;
  }

  printf("Enter Player Details:\n");

  if (!read_int("Player ID: ", &player_id) || player_id < 1 || player_id > 1000) {
    printf("Invalid player ID. Enter 1 to 1000.\n");
    return;
  }

  if (player_id_exists(player_id)) {
    printf("Player ID already exists.\n");
    return;
  }

  printf("Name: ");

  if (scanf(" %100[^\n]", name) != 1 || strlen(name) > MAX_NAME) {
    printf("Invalid name. Use 1 to 50 characters.\n");
    return;
  }

  if (!read_int("Role (1-Batsman, 2-Bowler, 3-All-rounder): ", &role) || role < 1 || role > 3) {
    printf("Invalid role.\n");
    return;
  }

  if (!read_int("Total Runs: ", &total_runs) || !read_float("Batting Average: ", &batting_average) || !read_float("Strike Rate: ", &strike_rate) || !read_int("Wickets: ", &wickets) || !read_float("Economy Rate: ", &economy_rate)) {
    printf("Invalid number.\n");
    return;
  }

  if (total_runs < 0 || batting_average < 0 || strike_rate < 0 || wickets < 0 || economy_rate < 0) {
    printf("Values cannot be negative.\n");
    return;
  }

  new_player = create_player(player_id, name, selected_team->name, role, total_runs, batting_average, strike_rate, wickets, economy_rate);

  if (new_player == NULL) {
    printf("Memory allocation failed.\n");
    return;
  }

  add_player_to_team(selected_team, new_player);

  printf("Player added successfully to Team %s!\n", selected_team->name);

}

void display_team_players() {

  int team_id;
  team *selected_team;
  player *current;

  if (!read_int("Enter Team ID: ", &team_id) || find_team(team_id) == NULL) {
    printf("Team not found.\n");
    return;
  }

  selected_team = find_team(team_id);
  current = selected_team->head;

  printf("Players of Team %s:\n", selected_team->name);
  print_table_header(0);

  while (current != NULL) {
    print_player_row(current, 0);
    current = current->next;
  }

  printf("Total Players: %d\n", selected_team->total_players);
  printf("Average Batting Strike Rate: %.2f\n", selected_team->average_strike_rate);

}

void display_teams_by_strike_rate() {

  team *sorted_teams[MAX_TEAMS];
  team *current_team;
  int index;
  int position;

  for (index = 0; index < team_count; index++) {
    sorted_teams[index] = &team_list[index];
  }

  for (index = 1; index < team_count; index++) {

    current_team = sorted_teams[index];
    position = index - 1;

    while (position >= 0 && sorted_teams[position]->average_strike_rate < current_team->average_strike_rate) {
      sorted_teams[position + 1] = sorted_teams[position];
      position--;
    }

    sorted_teams[position + 1] = current_team;

  }

  printf("Teams Sorted by Average Batting Strike Rate\n");
  printf("=========================================================\n");
  printf("%-4s %-15s %-12s %s\n", "ID", "Team Name", "Avg Bat SR", "Total Players");
  printf("=========================================================\n");

  for (index = 0; index < team_count; index++) {
    printf("%-4d %-15s %-12.2f %d\n", sorted_teams[index]->team_id, sorted_teams[index]->name, sorted_teams[index]->average_strike_rate, sorted_teams[index]->total_players);
  }

  printf("=========================================================\n");

}

void display_top_players() {

  int team_id;
  int role;
  int count;
  int printed = 0;
  team *selected_team;
  player *current;

  if (!read_int("Enter Team ID: ", &team_id) || find_team(team_id) == NULL) {
    printf("Team not found.\n");
    return;
  }

  selected_team = find_team(team_id);

  if (!read_int("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ", &role) || role < 1 || role > 3) {
    printf("Invalid role.\n");
    return;
  }

  if (!read_int("Enter number of players: ", &count) || count < 1) {
    printf("Invalid number of players.\n");
    return;
  }

  current = selected_team->role_head[role - 1];

  printf("Top %d %s players of Team %s:\n", count, role_names[role], selected_team->name);
  print_table_header(0);

  while (current != NULL && printed < count) {
    print_player_row(current, 0);
    current = current->next_in_role;
    printed++;
  }

  if (printed < count) {
    printf("Only %d players available.\n", printed);
  }

}

void sift_down(player *heap[], int size, int index) {

  int largest;
  int left;
  int right;
  player *temporary;

  while (1) {

    largest = index;
    left = 2 * index + 1;
    right = 2 * index + 2;

    if (left < size && heap[left]->performance_index > heap[largest]->performance_index) {
      largest = left;
    }

    if (right < size && heap[right]->performance_index > heap[largest]->performance_index) {
      largest = right;
    }

    if (largest == index) {
      return;
    }

    temporary = heap[index];
    heap[index] = heap[largest];
    heap[largest] = temporary;
    index = largest;

  }

}

void display_players_by_role() {

  player *heap[MAX_TEAMS];
  player *top_player;
  int role;
  int size = 0;
  int index;

  if (!read_int("Enter Role (1-Batsman, 2-Bowler, 3-All-rounder): ", &role) || role < 1 || role > 3) {
    printf("Invalid role.\n");
    return;
  }

  for (index = 0; index < team_count; index++) {
    if (team_list[index].role_head[role - 1] != NULL) {
      heap[size] = team_list[index].role_head[role - 1];
      size++;
    }
  }

  for (index = size / 2 - 1; index >= 0; index--) {
    sift_down(heap, size, index);
  }

  printf("%s players of all teams:\n", role_names[role]);
  print_table_header(1);

  while (size > 0) {

    top_player = heap[0];
    print_player_row(top_player, 1);

    if (top_player->next_in_role != NULL) {
      heap[0] = top_player->next_in_role;
    }

    else {
      size--;
      heap[0] = heap[size];
    }

    sift_down(heap, size, 0);

  }

}

void release_memory() {

  int index;
  player *current;
  player *next_player;

  for (index = 0; index < team_count; index++) {

    current = team_list[index].head;

    while (current != NULL) {
      next_player = current->next;
      free(current);
      current = next_player;
    }

  }

}

void display_menu() {

  printf("\n==============================================================================");
  printf("\nICC ODI Player Performance Analyzer");
  printf("\n==============================================================================");
  printf("\n1. Add Player to Team");
  printf("\n2. Display Players of a Specific Team");
  printf("\n3. Display Teams by Average Batting Strike Rate");
  printf("\n4. Display Top K Players of a Specific Team by Role");
  printf("\n5. Display all Players of specific role Across All Teams by performance index");
  printf("\n6. Exit");
  printf("\n==============================================================================\n");

}


int main() {

  int choice = 0;
  int index;
  team *selected_team;
  player *new_player;

  team_count = teamCount;

  for (index = 0; index < team_count; index++) {
    team_list[index].team_id = index + 1;
    snprintf(team_list[index].name, sizeof(team_list[index].name), "%s", teams[index]);
  }

  for (index = 0; index < playerCount; index++) {

    selected_team = find_team_by_name(players[index].team);

    if (selected_team == NULL) {
      continue;
    }

    new_player = create_player(players[index].id, players[index].name, players[index].team, role_from_text(players[index].role), players[index].totalRuns, players[index].battingAverage, players[index].strikeRate, players[index].wickets, players[index].economyRate);

    if (new_player == NULL) {
      printf("Memory allocation failed.\n");
      release_memory();
      return 1;
    }

    add_player_to_team(selected_team, new_player);

  }

  while (choice != 6) {

    display_menu();

    if (!read_int("Enter your choice: ", &choice)) {
      printf("Invalid input.\n");
      break;
    }

    switch (choice) {
      case 1:
        add_player();
        break;

      case 2:
        display_team_players();
        break;

      case 3:
        display_teams_by_strike_rate();
        break;

      case 4:
        display_top_players();
        break;

      case 5:
        display_players_by_role();
        break;

      case 6:
        break;

      default:
        printf("Invalid choice.\n");
    }

  }

  release_memory();

  printf("Exiting program\n");

  return 0;

}
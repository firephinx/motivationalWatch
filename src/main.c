#include <pebble.h>

static Window *s_main_window;
static TextLayer *s_time_layer;
static TextLayer *s_text_layer;
static GFont s_time_font;

const int nounCnt = 45;
const char *noun[]={"firetrucks","chewing gum","soaps","gardens","pencils","shirts","computers","video games","Carnegie Mellon",
                     "hacking","hackathons","fairy tales","melons","cats","glasses","clans","lights","fireworks","mice","dogs","bags",
                     "shoes","toothbrushes","yawns","floors","ceilings","skies","places","things","boxes","secrets","books","phones","people",
                     "hobbits","goblins","elves","rings","music","friendship","magic","ponies","fire extinguishers","search engines","achievements"};
const int negadjCnt = 30;
const char *negadj[]={"bad","desperate","terrible","indecisive","vague","cloudy","unfortunate","cantankerous","finicky","foolhardy","fussy","mean",
                      "unpredictable","silly","sneaky","patronizing","nasty","inflexible","harsh","lazy","cynical","messy","stagnant","stale",
                      "stressful","scary","repulsive","rainy","arrogant","needy"};
const int negvbCnt = 32;
const char *negvb[]={"despair","worry","dismay","disapprove","downgrade","disrespect","drop out","fail","expire","fear","gripe","grumble",
                     "hallucinate","hate","be lame","mess up","misbehave","objectify","be obliterated","offend","oppress","ostracize","perish",
                     "panic","procrastinate","recoil","retreat","scorn","shriek","slaughter","squabble","be nauseated"};
const int comparativesCnt = 34;
const char *comparatives[]={"happier","calmer","better","nicer","wittier","prouder","more delightful","bigger","more alive","more important",
                           "greener","redder","bluer","more purple","more yellow","more glamorous","fancier","chubbier","sweeter","younger","older",
                           "fluffier","warmer","yummier","stronger","jollier","more charming","more certain","more decisive","cooler","cuter",
                           "more grandiose","groovier","more magical"};
const int verbxCnt = 13;
const char *verbx[]={"look","seem","appear","act","become","end up","wax","stay","sound","remain","prove","get","taste"};
const int adverbCnt = 32;
const char *adverb[]={"positively","nicely","gracefully","loudly","intuitively","greatly","wonderfully","naturally","optimistically","openly",
                      "truly","truthfully","sweetly","unusually","quietly","powerfully","lovingly","wonderfully","fantastically","justly","easily",
                      "patiently","youthfully","stealthily","respectfully","reproachfully","tenderly","merrily","softly","magically","kindly","pretty"};
const int adjCnt = 31;
const char *adj[]={"good","lovely","wonderful","nice","agreeable","calm","beautiful","fluffy","happy","witty","small","red","blue","yellow","green",
                   "purple","orange","glamorous","glorious","fabulous","meaningful","beautiful","alive","active","interesting","attractive","careful",
                   "special","miniature","tall","short"};
const int verbCnt = 38;
const char *verb[]={"think","come","look","appear","run","lift","decide","mix","knit","gather","fix","accomplish","bubble","hammer","greet","enlist",
                    "hypothesize","fly","hypnotize","mesmerize","memorize","happen","analyze","amuse","dig","assemble","exist","laugh","remain",
                    "twist","eat","rise","smile","sneeze","spread","surprise","be unlocked","write"};
const int timesCnt = 11;
const char *times[]={"now","later","soon","tomorrow","yesterday","eventually","often","sometimes","forever","all the time","on rare occasions"};

int prevMin = -1;

static void update_speech() {
  // Create string that will be used to display whatever.
  char *output=malloc(500);
  int speechtype = rand()%3;
  if (speechtype == 0) {
    strcpy(output,"Do not ");
    strcat(output,negvb[rand()%negvbCnt]);
    strcat(output, " - ");
    strcat(output,comparatives[rand()%comparativesCnt]);
    strcat(output," ");
    strcat(output,noun[rand()%nounCnt]);
    strcat(output," shall ");
    strcat(output,verb[rand()%verbCnt]);
    strcat(output,"!");
  }
  else if (speechtype == 1) {
    strcpy(output,"");
    strcat(output,verb[rand()%verbCnt]);
    strcat(output," ");
    strcat(output,adverb[rand()%adverbCnt]);
    strcat(output,", ");
    strcat(output,noun[rand()%nounCnt]);
    strcat(output," ");
    strcat(output,verb[rand()%verbCnt]);
    strcat(output," ");
    strcat(output,adverb[rand()%adverbCnt]);
    strcat(output," ");
    strcat(output,adj[rand()%adjCnt]);
  }
  else if (speechtype == 2) {
    strcpy(output,"");
    strcat(output,noun[rand()%nounCnt]);
    strcat(output," may ");
    strcat(output,verbx[rand()%verbxCnt]);
    strcat(output," ");
    strcat(output,negadj[rand()%negadjCnt]);
    strcat(output," ");
    strcat(output,times[rand()%timesCnt]);
    strcat(output," but they ");
    strcat(output,verbx[rand()%verbxCnt]);
    strcat(output," ");
    strcat(output,comparatives[rand()%comparativesCnt]);
  }
  
  text_layer_set_text(s_text_layer, output);
  
  free(output);
}

static void update_time() {
  // Get a tm structure
  time_t temp = time(NULL); 
  struct tm *tick_time = localtime(&temp);

  // Create a long-lived buffer
  static char buffer[] = "00:00";

  // Write the current hours and minutes into the buffer
  if(clock_is_24h_style() == true) {
    // Use 24 hour format
    strftime(buffer, sizeof("00:00"), "%H:%M", tick_time);
  } else {
    // Use 12 hour format
    strftime(buffer, sizeof("00:00"), "%I:%M", tick_time);
  }
  
  int curMin = tick_time->tm_min;
  if (prevMin == -1) prevMin = curMin;
  else if (prevMin != curMin) {
    prevMin = curMin;
    update_speech();
  }
  
  // Display this time on the TextLayer
  text_layer_set_text(s_time_layer, buffer);
}

static void main_window_load(Window *window) {
  // Create time TextLayer
  s_time_layer = text_layer_create(GRect(0, 110, 144, 110));
  s_text_layer = text_layer_create(GRect(10, 0, 127, 120));
  text_layer_set_background_color(s_time_layer, GColorClear);
  text_layer_set_text_color(s_time_layer, GColorBlack);
  text_layer_set_background_color(s_text_layer, GColorClear);
  text_layer_set_text_color(s_text_layer, GColorBlack);
    
  // Create GFont
  s_time_font = fonts_load_custom_font(resource_get_handle(RESOURCE_ID_Bauer_Bodoni_Bold_52));
  
    
  // Improve the layout to be more like a watchface
  // Apply to TextLayer
  text_layer_set_font(s_time_layer, s_time_font);
  text_layer_set_text_alignment(s_time_layer, GAlignBottom);
  text_layer_set_font(s_text_layer, fonts_get_system_font(FONT_KEY_GOTHIC_24_BOLD));
  
  update_speech();

  // Add it as a child layer to the Window's root layer
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(s_text_layer));
}

static void main_window_unload(Window *window) {
    // Unload GFont
    fonts_unload_custom_font(s_time_font);
}

static void tick_handler(struct tm *tick_time, TimeUnits units_changed) {
  update_time();
}

static void window_load()
{
    window_stack_push(s_main_window, true);
    update_time();
}

static void init() {
// Create main Window element and assign to pointer
  s_main_window = window_create();

  // Set handlers to manage the elements inside the Window
  window_set_window_handlers(s_main_window, (WindowHandlers) {
    .load = main_window_load,
    .unload = main_window_unload
  });
  // Show the Window on the watch, with animated=true
  window_stack_push(s_main_window, true);
    
  // Register with TickTimerService
  tick_timer_service_subscribe(MINUTE_UNIT, tick_handler);
  
  // Set RNGesus
  srand(time(NULL));
}

static void deinit() {
    // Destroy Window
    window_destroy(s_main_window);
}
int main(void) {
  init();
  app_event_loop();
  deinit();
}
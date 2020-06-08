// Slightly more sophisticated Geiger-Müller counter software
//
// General idea: - Displays click count or average radioactivity in the past
//                 few seconds or minutes or..., or in a past interval
//               - Cpm or uSv/h
//               - Remember values depending on the size of your memory
//                 1 hour = 60*60*4 = 7200 bytes with 32-bit precision

//typedef unsigned long click_count_t; // 32-bit: prefer this value: safe!
typedef unsigned int click_count_t; // 16-bit: Why-not-value if you know what your're doing!
//typedef unsigned char click_count_t; // 8-bit: risky value!!!

const bool PARANOIA = true;

// We store all click sums during every second in an array
// Danger! The amount of memory available on your Arduino
//         may very well be limited!
// Example: 1 hour = 60*60 = 3600 seconds
const unsigned long MEMORY_SIZE = 60 * 60 + 1; // +1 is important!!
//const long MEMORY_SIZE = 60 + 1; // +1 is important!!
click_count_t values[MEMORY_SIZE];
long n_read_values;
long values_index;

click_count_t counts; //variable for GM Tube events
unsigned long previousMillis; //variable for measuring time

// Forces a value to be in an interval [0..max[
// "modulo-style"...
long normalize(long value, long max_excluded) {
  while (value < 0) {
    value += max_excluded;
  }

  while (value >= max_excluded) {
    value -= max_excluded;
  }

  return value;
}

click_count_t count_clicks_since_last_seconds(long n_seconds) {
  if (PARANOIA && n_seconds > MEMORY_SIZE - 1) {
    Serial.println(F("Warning! count_clicks_since_last_seconds(): n_seconds too big! Trying to continue with a smaller value..."));
    n_seconds = MEMORY_SIZE - 1;
  }

  /*if (values_index == 0) {
    Serial.println("X");
    Serial.println(MEMORY_SIZE);
    Serial.println(values_index);
    Serial.println(normalize(((long)values_index) - 1, MEMORY_SIZE));
    Serial.println(normalize(((long)values_index) - 1 - n_seconds, MEMORY_SIZE));
    Serial.println(values[normalize(values_index - 1, MEMORY_SIZE)]);
    Serial.println(values[normalize(values_index - 1 - n_seconds, MEMORY_SIZE)]);  
  }*/
  
  return values[normalize(values_index - 1, MEMORY_SIZE)] - values[normalize(values_index - 1 - n_seconds, MEMORY_SIZE)];
}

double avg_clicks_per_second_since_last_seconds(long n_seconds = MEMORY_SIZE - 1) {
  if (PARANOIA && n_seconds > MEMORY_SIZE - 1) {
    Serial.println(F("Warning! avg_clicks_per_second_since_last_seconds(): n_seconds too big! Trying to continue with a smaller value..."));
    n_seconds = MEMORY_SIZE - 1;
  }
  
  click_count_t min_possible_value = min(n_read_values, n_seconds);

  return ((double)count_clicks_since_last_seconds(min_possible_value)) / min_possible_value;
}

void impulse() {
  counts++;
}

extern int freeMemory();

const int ONE_uSv_PER_HOUR_IN_CPM = 151; // Tube dependend!
const double CONVERT_CPM_TO_uSv_PER_HOUR = 60.0 / ONE_uSv_PER_HOUR_IN_CPM;

void setup() {
  counts = 0;
  Serial.begin(9600);
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), impulse, FALLING); //define external interrupts
  
  Serial.println(F("Start counter"));

  for (int i = 0; i < MEMORY_SIZE; i++) {
    values[i] = 0;
  }

  n_read_values = 0;
  values_index = 0;

  // Just printing amount of available memory
  Serial.print(F("Free memory = "));
  Serial.println(freeMemory());

  // The following calculationn shows that you need to store 32-bit values
  // if you may be facing high values of radiation...
  
  // Saturation at approx. 1mSv/h
  unsigned long saturation_at_uSv_per_hour = 1000;
  unsigned long saturation_at_cpm = saturation_at_uSv_per_hour * ONE_uSv_PER_HOUR_IN_CPM;
  unsigned long max;
  switch (sizeof(click_count_t)) {
    case 1: max = 0xFF;
      break;
    case 2: max = 0xFFFF;
      break;
    case 4: max = 0xFFFFFFFF;
      break; 
  }

  double saturation_after = ((double)max) / saturation_at_cpm * 60;
  Serial.print(sizeof(click_count_t)*8);
  Serial.print(F("-bit oriented: Will overflow after "));
  Serial.print(saturation_after);
  Serial.print(F(" seconds of saturation at "));
  Serial.print(saturation_at_uSv_per_hour);
  Serial.println(F(" uSv/h..."));


  Serial.println();
  Serial.println(normalize(-1, MEMORY_SIZE));
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - previousMillis > 1000) { // 1 second in ms
    previousMillis = currentMillis;
    values[values_index] = counts;
    values_index = normalize(values_index + 1, MEMORY_SIZE);

    //if (n_read_values != MEMORY_SIZE - 1) {
    if (n_read_values != MEMORY_SIZE) {
      n_read_values++;
    }

    Serial.print(n_read_values);
    Serial.print(" ");
    Serial.print(counts);
    Serial.print(" ");
    Serial.print(values_index);
    Serial.print(" ");
    Serial.print(count_clicks_since_last_seconds(1));
    Serial.print(" ");
    Serial.print(count_clicks_since_last_seconds(10));
    Serial.print(" ");
    Serial.print(count_clicks_since_last_seconds(60)); // cpm!
    Serial.print(" ");
    Serial.print(count_clicks_since_last_seconds(10*60));
    Serial.print(" ");
    Serial.print(count_clicks_since_last_seconds(60*60));
    Serial.print(" ");
    Serial.print(avg_clicks_per_second_since_last_seconds(60*60) * 60, 4);
    Serial.print(" ");
    Serial.print(avg_clicks_per_second_since_last_seconds(60*60) * CONVERT_CPM_TO_uSv_PER_HOUR, 4);
    Serial.print(F(" uSv/h"));
    

    // 1 min test
    /*Serial.print(avg_clicks_per_second_since_last_seconds(60) * 60, 4);
    Serial.print(" ");
    Serial.print(avg_clicks_per_second_since_last_seconds(60) * CONVERT_CPM_TO_uSv_PER_HOUR, 4);
    Serial.print(F(" uSv/h"));
    */
    Serial.println();
  }
}

#ifdef __arm__
// should use uinstd.h to define sbrk but Due causes a conflict
extern "C" char* sbrk(int incr);
#else  // __ARM__
extern char *__brkval;
#endif  // __arm__
 
int freeMemory() {
  char top;
#ifdef __arm__
  return &top - reinterpret_cast<char*>(sbrk(0));
#elif defined(CORE_TEENSY) || (ARDUINO > 103 && ARDUINO != 151)
  return &top - __brkval;
#else  // __arm__
  return __brkval ? &top - __brkval : &top - __malloc_heap_start;
#endif  // __arm__
}

    

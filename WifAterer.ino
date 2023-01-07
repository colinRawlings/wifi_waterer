#include <memory>
#include <vector>

auto val = std::make_shared<int>(2);

std::pair<int, int> pair{1, 2};
std::vector<int> vec{1, 2, 3};

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED_BUILTIN,
               HIGH);             // turn the LED on (HIGH is the voltage level)
  delay(1000);                    // wait for a second
  digitalWrite(LED_BUILTIN, LOW); // turn the LED off by making the voltage LOW
  delay(1000);                    // wait for a second
}
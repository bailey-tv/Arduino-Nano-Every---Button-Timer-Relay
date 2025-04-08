# Arduino Relay Control with Cooldown and LED Indicator

This project demonstrates how to control a **5V relay** using an **Arduino Nano Every** and a **momentary push button**. The system includes a built-in cooldown period and an LED indicator to signal system states.

## 🛠️ Hardware Used

- **Arduino Nano Every**
- **AEDIKO Relay Module** (5V, 1 Channel, Optocoupler isolated, High/Low trigger)
- **Momentary push button**
- **LED** with 220Ω resistor (optional)
- **Wires & breadboard**

## ⚙️ How It Works

This system operates in three main states:

1. **Waiting for Press**
   - LED is ON (system ready).
   - Relay is OFF.
   - Pressing the button activates the relay.

2. **Relay Active**
   - Relay is ON for **5 seconds**.
   - LED turns OFF while active.

3. **Cooldown**
   - After the relay deactivates, the system enters a **15-second cooldown**.
   - During this time, the LED remains OFF and button presses are ignored.

After cooldown, the system resets and is ready for another press.

## ⏱️ Timing Configuration

```cpp
const unsigned long kRelayActiveTime = 5000;  // Relay on time (5s)
const unsigned long kCooldownTime    = 15000; // Cooldown duration (15s)
```

You can modify these values to change the active and cooldown durations.

## 🧰 Wiring Diagram

| Component     | Arduino Pin | Notes                           |
|--------------|--------------|----------------------------------|
| Push Button  | D2           | Uses `INPUT_PULLUP`, connect one side to GND |
| Relay IN     | D3           | Triggers the relay (LOW or HIGH depending on module) |
| LED + Resistor | D4         | Optional, for visual feedback    |
| Relay VCC    | 5V           | Power from Arduino               |
| Relay GND    | GND          | Ground                          |

> 🧠 **Note:** The AEDIKO Relay supports both HIGH or LOW trigger. This code assumes **HIGH trigger** (i.e., setting the relay pin HIGH activates the relay). Adjust `digitalWrite(relayPin, HIGH/LOW)` accordingly if your module is configured for LOW trigger.

## 🔌 Setup Instructions

1. Connect components as described above.
2. Upload the provided sketch to your **Arduino Nano Every** using the Arduino IDE.
3. Open the Serial Monitor (optional) to observe runtime behavior.
4. Press the button to trigger the relay.

## 🧠 How the Code Works

The code uses a simple **finite state machine** (`enum SystemState`) to manage the three operating modes:

- `WAITING_FOR_PRESS`
- `RELAY_ACTIVE`
- `COOLDOWN`

It uses `millis()` for **non-blocking timing**, allowing seamless transitions between states without using `delay()`.

```cpp
unsigned long currentMillis = millis();
if (currentMillis - stateStartTime >= timeLimit) { ... }
```

This keeps the system responsive and accurate.

## ✅ Use Cases

- Basic relay control with debounce/cooldown logic
- Garage door controller
- Power cycling systems
- Any system needing a timed relay output with lockout

## 📌 License

MIT License — Feel free to use, modify, and share!

const int ldr = A0;

int lightlevel = 0;
// String charged = "init";
// int threshold = 750;

void setup() {
    pinMode(ldr, INPUT);

    delay(5000);
}

void loop() {
    lightlevel = analogRead(ldr);
    // Particle.publish("status", String(lightlevel));
    Particle.publish("light-meter", String(lightlevel), PRIVATE);
    delay(3000);

    // if(lightlevel < threshold && charged != "true") {
    //     Particle.publish("status", "charged (" + String(lightlevel) + ")");
    //     charged = "true";
    // }
    // else if(lightlevel > threshold && charged != "false") {
    //     Particle.publish("status", "charging (" + String(lightlevel) + ")");
    //     charged = "false";
    // }

    // delay(3000);
}

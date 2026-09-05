/*
 * ejercicio1.c
 *
 * Created: 26/08/2026 15:00:54
 * Author : Santiago
 */ 

#ifndef F_CPU
#define F_CPU 16000000UL // Ajustá a la frecuencia del reloj de tu proyecto en Proteus (ej. 16 MHz)
#endif

#include <avr/io.h>
#include <util/delay.h>

int main(void)
{
	// 1. Configuración de E/S
	DDRB |= (1 << PORTB0) | (1 << PORTB1);  // PB0 y PB1 como salidas
	DDRD &= ~((1 << PORTD2) | (1 << PORTD3)); // PD2 y PD3 como entradas

	// 2. Estado inicial
	PORTB |= (1 << PORTB0) | (1 << PORTB1);   // Encender PB0 y PB1 (Punto a)
	PORTD |= (1 << PORTD2) | (1 << PORTD3);   // Activar resistencias pull-up internas en PD2 y PD3

	// 3. Esperar a que se presione alguno de los dos pulsadores (vía polling en PIND)
	// Con pull-up, el pin lee '0' (LOW) al presionar el pulsador.
	while ((PIND & (1 << PIND2)) && (PIND & (1 << PIND3))) {
		// Mientras ambos estén en '1' (sin presionar), se queda esperando acá.
	}

	// 4. Parpadeo alternado indefinido tras detectar la primera pulsación (Punto b y c)
	while (1) {
		// Estado 1: PB0 encendido, PB1 apagado
		PORTB = (PORTB & ~(1 << PORTB1)) | (1 << PORTB0);
		_delay_ms(250);

		// Estado 2: PB0 apagado, PB1 encendido
		PORTB = (PORTB & ~(1 << PORTB0)) | (1 << PORTB1);
		_delay_ms(250);
	}

	return 0;
}

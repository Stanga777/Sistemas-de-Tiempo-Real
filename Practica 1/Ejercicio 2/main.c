/*
 * Ejercicio2.c
 *
 * Created: 05/09/2026 10:37:14
 * Author : Santiago
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

volatile int parpadeo_iniciado = 0;
volatile uint8_t contador_ms = 0;

// Interrupción por comparación de Timer0 (cada 10 ms)
ISR(TIMER0_COMPA_vect) {
	if (parpadeo_iniciado) {
		contador_ms++;
		if (contador_ms >= 25) { // 25 * 10 ms = 250 ms
			contador_ms = 0;
			// Alternar los estados de PB0 y PB1
			PORTB ^= (1 << PORTB0) | (1 << PORTB1);
		}
	}
}

int main(void) {
	// Configurar PB0 y PB1 como salidas
	DDRB |= (1 << PORTB0) | (1 << PORTB1);
	
	// Configurar PD2 y PD3 como entradas con pull-up
	DDRD &= ~((1 << PORTD2) | (1 << PORTD3));
	PORTD |= (1 << PORTD2) | (1 << PORTD3);

	// Estado inicial: Ambos LEDs encendidos
	PORTB |= (1 << PORTB0) | (1 << PORTB1);

	// Configurar Timer0 en modo CTC
	TCCR0A |= (1 << WGM01);
	
	// Prescaler 1024
	TCCR0B |= (1 << CS02) | (1 << CS00);
	
	// Valor de comparación para 10 ms (156 ticks)
	OCR0A = 155;
	
	// Habilitar interrupción por Compare Match A
	TIMSK0 |= (1 << OCIE0A);

	// Habilitar interrupciones globales
	sei();

	while (1) {
		// Polling para detectar el inicio por pulsador
		if (!parpadeo_iniciado) {
			if (!(PIND & (1 << PIND2)) || !(PIND & (1 << PIND3))) {
				parpadeo_iniciado = 1;
				// Estado inicial del parpadeo: PB0 encendido, PB1 apagado
				PORTB |= (1 << PORTB0);
				PORTB &= ~(1 << PORTB1);
				contador_ms = 0;
			}
		}
	}

	return 0;
}




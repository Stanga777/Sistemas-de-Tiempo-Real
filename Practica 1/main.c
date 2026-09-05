/*
 * Ejercicio3.c
 *
 * Created: 05/09/2026 19:40:53
 * Author : Santiago
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

void ADC_init(void) {
	// Referencia AVCC (5V) con capacitor en AREF, Selección de canal ADC0 (MUX3:0 = 0000)
	ADMUX = (1 << REFS0);
	
	// Habilitar ADC y establecer Prescaler de 128 (16 MHz / 128 = 125 kHz)
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t ADC_read(void) {
	// Iniciar conversión por Polling
	ADCSRA |= (1 << ADSC);
	
	// Esperar a que la conversión finalice (ADSC pasa a 0)
	while (ADCSRA & (1 << ADSC));
	
	return ADC; // Retorna el valor de 10 bits (0x000 a 0x3FF)
}

void mostrar_hex_multiplexado(uint16_t valor) {
	// Separar en 4 nibbles hexadecimales
	uint8_t d0 = valor & 0x0F;         // Menos significativo
	uint8_t d1 = (valor >> 4) & 0x0F;
	uint8_t d2 = (valor >> 8) & 0x0F;
	uint8_t d3 = (valor >> 12) & 0x0F; // Más significativo (será 0 para 10 bits)

	// Enviar par d0-d1 al primer Latch (Habilitado por PC1)
	PORTB = (d1 << 4) | d0;
	PORTC |= (1 << PORTC1);  // LE alto
	_delay_us(1);
	PORTC &= ~(1 << PORTC1); // LE bajo

	// Enviar par d2-d3 al segundo Latch (Habilitado por PC2)
	PORTB = (d3 << 4) | d2;
	PORTC |= (1 << PORTC2);  // LE alto
	_delay_us(1);
	PORTC &= ~(1 << PORTC2); // LE bajo
}

int main(void) {
	// Puerto B completo como salida (bus de datos para displays)
	DDRB = 0xFF;
	
	// PC1 y PC2 como salidas para Latch Enable (LE)
	DDRC |= (1 << PORTC1) | (1 << PORTC2);
	
	ADC_init();

	while (1) {
		uint16_t valor_adc = ADC_read();
		mostrar_hex_multiplexado(valor_adc);
		_delay_ms(10);
	}

	return 0;
}


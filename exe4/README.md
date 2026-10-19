# EXE 4

![](diagram.png)

- Arquivo: `exe4/main.c`
- Teste: Verifica os pinos dos LEDs

> Objetivo: Trabalhar com filas entre IRQ e `tasks`

Repita o mesmo que o código já faz, só que agora para o LED G. Note o uso da fila no callback do botão! Você terá que usar `xQueueSendFromISR`.

#ifndef EXCEPTION_H
#define EXCEPTION_H

#ifndef MACRO_ONLY

extern void exception_init(void);
extern void exception_irq_handler(void);
extern void exception_int_ena(int no, void (*callback)(void));
extern void exception_int_dis(int no);

#endif  /* MACRO_ONLY */

#endif /* EXCEPTION_H */

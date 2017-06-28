;floors.scm

; Baker, Cooper, Fletcher, Miller and Smith all live on different floors of an apartment house
; that contains five floors.
;
; Baker does not live on the top floor.
; Cooper does not live on the Bottom floor.
; Fletcher does not live on the top or the bottom floor.
; Miller lives on a higher floor than does Cooper.
; Smith does not live on a floor adjacent to Fletcher
; Fletcher does not live on a floor adjacent to Cooper.
; Where does every one live?

(load "amb.scm")

(define floors '(1 2 3 4 5))

(let*
(
(people (permute '(baker cooper fletcher miller smith)))
)

(required (notassociated? 'baker    people '5  floors))
(required (notassociated? 'cooper   people '1  floors))
(required (notassociated? 'fletcher people '5  floors))
(required (notassociated? 'fletcher people '1  floors))
(required (above?         'miller   people 'cooper   people))
(required (notadjacent?   'smith    people 'fletcher people))
(required (notadjacent?   'fletcher people 'cooper   people))

(PrintSolution floors people)
(newline)
(amb) ;;; any more??????
)

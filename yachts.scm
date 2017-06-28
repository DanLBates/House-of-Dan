;yachts.scm

; Mary Ann Moore's father has a yacht and so has each of his four friends:
; Colonel Downing, Mr. Hall, Sir Barnacle Hood, and Dr. Parker. Each of
; the five also has one daughter and each has named his yacht after the daughter
; of one of the others. Sir Barnacle's yacht is the Gabrielle, Mr. Moore
; owns the Lorna; Mr. Hall the Rosalind. The Melissa, owned by Colonel
; Downing is named after Sir Barnacle's daughter. Gabrielle's father owns
; the yacht that is named after Dr. Parker's daughter. Who is Lorna's father?

(load "amb.scm")

(define fathers '(moore downing hall hood parker))

(define femalenames '(mary gabrielle lorna rosalind melissa))

(let*

(
;;;;;permuted sets
(yachts    (permute femalenames))
(daughters (permute femalenames))
;;;;;local variables we need
(gabriellesidx 0)
(parkersidx 0)
)

;;;;;requirements
; Mary Ann Moore's father has a yacht
(required (associated?    'moore fathers 'mary daughters))
(required (notassociated? 'moore fathers 'mary yachts))
;Sir Barnacle's yacht is the Gabrielle
(required (associated?    'hood  fathers 'gabrielle yachts))
(required (notassociated? 'hood  fathers 'gabrielle daughters))
; Mr. Moore owns the Lorna
(required (associated?    'moore fathers 'lorna     yachts))
(required (notassociated? 'moore fathers 'lorna     daughters))
; Mr. Hall the Rosalind
(required (associated?    'hall fathers 'rosalind yachts))
(required (notassociated? 'hall fathers 'rosalind daughters))
; The Melissa, owned by Colonel Downing is named after Sir Barnacle's daughter.
(required (associated?    'downing fathers 'melissa yachts))
(required (associated?    'hood    fathers 'melissa daughters))
; Gabrielle's father owns the yacht that is named after Dr. Parker's daughter
(set! gabriellesidx (idx 'gabrielle daughters))
(set! parkersidx    (idx 'parker    fathers))
(required (eqv? (list-ref yachts gabriellesidx)(list-ref daughters parkersidx)))
(required (notassociated? 'parker fathers 'gabrielle daughters))

;;;;;Solution
(display "Fathers Daughters Yachts") (newline)
(display "========================") (newline)
(PrintSolution fathers daughters yachts)
(newline)
(display "lorna's father is ")
(display (list-ref fathers (idx 'lorna daughters)))
(newline)
(amb) ;;???any more
)
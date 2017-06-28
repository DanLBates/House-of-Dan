;;;;; lolla.scm

;;;;;        The Lollapalooza marriage

;;;;;R1      Dolly is married to Fred
;;;;;R2      Polly is not married to Mick
;;;;;R3      Holly is not married to Mick.
;;;;;R4      Mick is Gerard
;;;;;R5      George is not surnamed Grant
;;;;;R6      Black is not Bill
;;;;;R7      Fred is not Atkinson
;;;;;R8      George is not surnamed Atkinson
;;;;;R9      George drives 1937 Jaguar SS100 Roadster
;;;;;R10     Blacks drive a 1935 Mecedes-Benz 500K
;;;;;R11     lolly was in mercedes-benz
;;;;;R12     The Grants drive a silver arrow
;;;;;R13     Polly does not drive a Cord
;;;;;R14     Bill does not drive a 1938 Cadillac V-16 Coupe
;;;;;R15     Bill does not drive a 1933 Pierce Silver arrow

(load "amb.scm")

(define brides   '(dolly  holly  lolly  molly  polly))

(let* ((grooms    (permute '(fred          bill      boris   george  mick))))
    (required (associated? 'dolly brides 'fred grooms))                 ;R1
    (required (notassociated? 'polly brides 'mick grooms))              ;R2
    (required (notassociated? 'holly brides 'mick grooms))              ;R3
(let* ((surnames  (permute '(atkinson      black     bower   gerard  grant))))
    (required (associated? 'mick grooms 'gerard surnames))              ;R4
    (required (notassociated? 'george grooms 'grant surnames))          ;R5
    (required (notassociated? 'bill grooms 'black surnames))            ;R6
    (required (notassociated? 'fred grooms 'atkinson surnames))         ;R7
    (required (notassociated? 'george grooms 'atkinson surnames))       ;R8
(let* ((cars      (permute '(silverarrow   mercedes  cord    jaguar  cadillac))))
    (required (associated? 'george grooms 'jaguar cars))                ;R9
    (required (associated? 'black surnames 'mercedes cars))             ;R10
    (required (associated? 'lolly brides 'mercedes cars))               ;R11
    (required (associated? 'grant surnames 'silverarrow cars))          ;R12
    (required (notassociated? 'polly brides 'cord cars))                ;R13
    (required (notassociated? 'bill grooms 'cadillac cars))             ;R14
    (required (notassociated? 'bill grooms 'silverarrow cars))          ;R15
;;;;;
;;;;;Past Filtering, so print the solution and see if there are any more.
;;;;;
    (PrintSolution brides grooms surnames cars)
    (newline)
    (amb)                  ;;;;; are there any more solutions
)))

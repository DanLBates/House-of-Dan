;sorority.scm

; Five school girls sat for an examination. Their parents -- so they thought --
; showed an undue degree of interest in the result. They therefore agreed that in
; writting home about the examination, each girl should make one true statement and
; one untrue statement. The relevant passages from there letters.

; Betty: "Kitty was second in the examination. I was only third."
; Ethel: "You'll be glad to hear that I was on top. Joan was second."
; Joan:  "I was third, and poor old Ethel was bottom."
; Kitty: "I came out second. Mary was only fourth."
; Mary:  "I was fourth. Top place was taken by Betty"

; What in fact was the order in which the girls were placed?

(load "amb.scm")

(define scores '(first second third fourth fifth))

(let*
(
(girls (permute '(betty ethel joan kitty mary)))
)
;;;;;requirements
(required (exor (associated? 'kitty girls 'second scores)
                (associated? 'betty girls 'third  scores)))
(required (exor (associated? 'ethel girls 'first  scores)
                (associated? 'joan  girls 'second scores)))
(required (exor (associated? 'joan  girls 'third  scores)
                (associated? 'ethel girls 'fifth  scores)))
(required (exor (associated? 'kitty girls 'second scores)
                (associated? 'mary  girls 'fourth scores)))
(required (exor (associated? 'mary  girls 'fourth scores)
                (associated? 'betty girls 'first  scores)))

;;;;;Print the Solution
(display "Betty: Kitty was second in the examination. I was only third.")
                                                               (newline)
(display "Ethel: You'll be glad to hear that I was on top. Joan was second.")
                                                               (newline)
(display "Joan:  I was third, and poor old Ethel was bottom.") (newline)
(display "Kitty: I came out second. Mary was only fourth.")    (newline)
(display "Mary:  I was fourth. Top place was taken by Betty")  (newline)
(newline)
(display "GIRL  SCORE") (newline)
(PrintSolution girls scores)
(newline)
(amb)       ;; ?More solutions let's look and see
)
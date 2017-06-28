;einstein.scm

;;;;;There are five houses in a row in different colors.
;;;;;In each house lives a person with a different nationality.
;;;;;The five owners drink a different drink,
;;;;;smoke a different brand of cigar
;;;;;and keep a different pet, one of which is a Walleye Pike.

;;;;;  1. The Brit lives in the red house.
;;;;;  2. The Swede keeps dogs as pets.
;;;;;  3. The Dane drinks tea.
;;;;;  4. The green house is on the left of the white house.
;;;;;           4. has to be interpreted as immediatly to the left of
;;;;;           or there is more than one solution
;;;;;  5. The green house owner drinks coffee.
;;;;;  6. The person who smokes Pall Malls keeps birds.
;;;;;  7. The owner of the yellow house smokes Dunhills.
;;;;;  8. The man living in the house right in the center drinks milk.
;;;;;  9. The man who smokes Blends lives next to the one who keeps cats.
;;;;; 10. The Norwegian lives in the first house.
;;;;; 11. The man who keeps horses lives next to the one who smokes Dunhills.
;;;;; 12. The owner who smokes Bluemasters drinks beer.
;;;;; 13. The German smokes Princes.
;;;;; 14. The Norwegian lives next to the blue house.
;;;;; 15. The man who smokes Blends has a neighbor who drinks water.
;;;;;           15. has to be interpreted as lives adjacent to.  or there
;;;;;           is more than one solution.
;;;;;The question is-- who owns the fish?

(load "amb.scm")

(define houseposition '(1 2 3 4 5))
(let* ((nationalities (permute '(norwegian brit swede dane german))))
    (required (associated? 'norwegian nationalities '1 houseposition)) ;10
(let* ((drinks (permute '(coffee milk beer tea water))))
    (required (associated? 'dane nationalities 'tea drinks))       ;3
    (required (associated? 'milk drinks '3 houseposition))         ;8
(let* ((housecolor (permute '(blue green white red yellow))))
    (required (associated?   'brit nationalities 'red housecolor)) ;1
    (required (just-left-of? 'green housecolor 'white housecolor)) ;4
    (required (associated?   'green housecolor 'coffee drinks))    ;5
    (required (adjacent? 'norwegian nationalities 'blue housecolor)) ;14
(let* ((pets (permute '(fish birds dogs horses cats))))
    (required (associated? 'swede nationalities 'dogs pets))       ;2
(let* ((smokes (permute '(blends pallmalls bluemasters dunhills princes))))
    (required (associated? 'pallmalls smokes 'birds pets))         ;6
    (required (associated? 'yellow housecolor 'dunhills smokes))   ;7
    (required (adjacent?   'blends smokes 'cats pets))             ;9
    (required (adjacent?   'horses pets 'dunhills smokes))         ;11
    (required (associated? 'bluemasters smokes 'beer drinks))      ;12
    (required (associated? 'german nationalities 'princes smokes)) ;13
    (required (adjacent?   'blends smokes 'water drinks))          ;15

(PrintSolution houseposition housecolor nationalities smokes drinks pets)
(newline)
(display "The ")
(display (list-ref nationalities (idx 'fish pets)))
(display " owns the fish.")
(newline)
(amb) ;;???any more
)))))
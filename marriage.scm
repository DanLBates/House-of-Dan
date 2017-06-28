;;marriage.pro
;;
;;        a marriage problem
;;        5 men:                          [paul, rob,   stan, vern, wally]
;;        married 1 on 1 5 women:         [anne, cathy, eve,  fran, ida]
;;        each on a different weekday:    [mon,  tue,   wed,  thu,  fri]
;;                such that:
;;        anne married on mon
;;        anne not married to wally
;;        stan married on wed
;;        rob married on fri
;;        rob not married to ida
;;        vern married to fran
;;        vern married day after eve
;;
(load "amb.scm")

(define week   '(mon tue wed thu fri))

(let*
(
(grooms (permute '(paul rob   stan vern wally)))
(brides (permute '(anne cathy eve  fran ida)))
)
;;        anne married on mon
    (required (associated? 'anne brides 'mon week))
;;        anne not married to wally
    (required (notassociated? 'anne brides 'wally grooms))
;;        stan married on wed
    (required (associated? 'stan grooms 'wed week))
;;        rob married on fri
    (required (associated? 'rob grooms 'fri week))
;;        rob not married to ida
    (required (notassociated? 'rob grooms 'ida brides))
;;        vern married to fran
    (required (associated? 'vern grooms 'fran brides))
;;        vern married day after eve
    (required (just-after? 'vern grooms 'eve brides))

        (PrintSolution week grooms brides)
        (newline)
        (amb-more)
)
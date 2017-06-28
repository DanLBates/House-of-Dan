;;;;amb.scm
;;;;
;;;; This was lifted from teach yourself scheme in fixnum
;;;; number of days by Dan Bates 20050721. I understand parts
;;;; of it. Other parts seem to be magic. It basically uses
;;;; the call/cc to implement a backtracking
;;;;
;;;; required is good for solving logic puzzles
;;;;
;;;; 20060523 Dan Bates changed order in associated? and notassociated?

(define amb-fail '*)

(define call/cc call-with-current-continuation)

(define initialize-amb-fail
    (lambda ()
        (set! amb-fail
            (lambda ()
                (error "amb tree exhausted")))))

(initialize-amb-fail)

(define amblist
    (lambda (alts...)
        (let ((+prev-amb-fail amb-fail))
            (call/cc
                (lambda (+sk)
                    (map (lambda (alt)
                        (call/cc
                            (lambda (+fk)
                                (set! amb-fail
                                    (lambda ()
                                        (set! amb-fail +prev-amb-fail)
                                        (+fk 'fail)))
                                    (+sk alt))))
                                alts...)
                            (+prev-amb-fail))))))

;;;;; Make the traditional form available as well
;;now (amb) works as well as (amb-more)
(define (amb . ls) (amblist ls))

(define (amb-more) (amblist '()))

(define required
    (lambda (pred)
        (if (not pred) (amb-more))))

;;;;;
;;;;; more logic puzzle helper this returns a list of whatever is
;;;;; in start that is not in minus
; Ex. (set-difference '(0 1 2 3 4 5 6 7 8 9 10) '(4 2 0 6 8 10)) ==> (1 3 5 7 9)
;;;;;
(define (set-difference start minus)
  (cond ((null? minus) start)
        ((null? start) '())
        ((not (memq (car start) minus))
              (cons (car start) (set-difference (cdr start) minus)))
        (else (set-difference (cdr start) minus))))

;;;;;
;;;;; make syntax easier to use for set-difference
;;;;; Ex: (select-from '(0 1 2 3 4 5 6 7 8 9 10) 4 2 0 6 8 10) ==> (1 3 5 7 9)
;;;;; numbers are self evaluating
;;;;; Ex2: note: must use let* and not let
;;;;;(let* ((JohnsCar (amblist (select-from Cars)))
;;;;;       (FredsCar (amblist (select-from Cars JohnsCar)))
;;;;;       (MarysCar (amblist (select-from Cars JohnsCar FredsCar)))
;;;;;       (LukesCar (amblist (select-from Cars JohnsCar FredsCar MarysCar))))
(define (select-from start . minus)
  (set-difference start minus))

;;;;;
;;;;; More tools for puzzle solving. The first of these is permute
;;;;; which give it a list it returns an amb permuted list of the same
;;;;; elements
;;;;;
(define (permute sel)
    (if (null? sel) '()
        (let*   (
                (elem (amblist sel))
                (newsel (set-difference sel (list elem)))
                )
            (append (list elem) (permute newsel))
        )
    )
)

;;;;;
;;;;; secondly associated? which tells is an argument1 in list1 is
;;;;; in the same position as argument2 is in list 2.
;;;;;
(define (associated? arg1 set1 arg2 set2)
    (cond   ((null? set1) #f)
            ((null? set2) #f)
            ((and (eq? arg1 (car set1)) (eq? arg2 (car set2))) #t)
            (else (associated? arg1 (cdr set1) arg2 (cdr set2)))
    )
)

(define (notassociated? arg1 set1 arg2 set2)
        (not (associated? arg1 set1 arg2 set2)))

(define (associatedtest? arg1 set1 arg2 set2)
    (cond ((not (memq arg1 set1)) (display `(,arg1 not in ,set1)))
           ((not (memq arg2 set2)) (display `(,arg2 not in ,set2)))
           (else #t)))

(define (notassociatedtest? arg1 set1 arg2 set2)
        (associatedtest? arg1 set1 arg2 set2))

;;;;;
;;;;; idx where in a set an ordinal is an element or #f if it is not there
;;;;;
(define (idx element set) (idx2 element set 0))

(define (idx2 element set n)
    (cond   ((null? set) #f)
            ((eq? element (car set)) n)
            (else (idx2 element (cdr set) (+ n 1)))
    )
)

;;;;;
;;;;; a Generic Print Solution for logic puzzle solving
;;;;;
(define (PrintSolution . ls) (PrintSolution2 ls))

(define (PrintSolution2  ls)
    (if (null? (car ls))
        'done
        (begin
        (PrintTheCars ls)                   ;;cars are one line
        (PrintSolution2 (ListOfCdrs ls))
        )
    )
)

(define (PrintTheCars ls)
    (if (null? ls)
        (newline)
        (begin
        (display (caar ls))
        (display " ")
        (PrintTheCars (cdr ls))
        )
    )
)

(define (ListOfCdrs ls)
    (if (null? ls)
        '()
        (cons (cdar ls) (ListOfCdrs (cdr ls)))
    )
)

;;;within a particular set are elem1 and elem2 adjacent?
(define (adjacent? elem1 set1 elem2 set2)
    (= (abs (- (idx elem1 set1) (idx elem2 set2))) 1)
)

;;;;within a particular set are elem1 and elem2 not adjacent?
(define (notadjacent? elem1 set1 elem2 set2)
    (> (abs (- (idx elem1 set1) (idx elem2 set2))) 1)
)

;;;;;more ordered relationship leftof? or below?
(define (leftof? elem1 set1 elem2 set2)
    (< (idx elem1 set1) (idx elem2 set2))
)
(define below? leftof?)

;;;;;how about rightof? or above?
(define (rightof? elem1 set1 elem2 set2)
    (> (idx elem1 set1) (idx elem2 set2))
)
(define above? rightof?)

;just-left-of? is elem1 immediatly before elem2
(define (just-left-of? elem1 set1 elem2 set2)
    (and (leftof?   elem1 set1 elem2 set2)
         (adjacent? elem1 set1 elem2 set2)
    )
)

(define just-below? just-left-of?)
(define just-before? just-left-of?)

;just-right-of is elem1 immediatly after elem2
(define (just-right-of? elem1 set1 elem2 set2)
    (and (rightof?  elem1 set1 elem2 set2)
         (adjacent? elem1 set1 elem2 set2)
    )
)

(define just-above? just-right-of?)
(define just-after? just-right-of?)

;;;;;for the situation where two statements are made with one
;;;;;   and only one being true
(define (exor a b)      ;;exclusive or
    (if a
        (not b)
        b
    )
)

;;;;;This is from an old implementation which I have moved beyond. This is
;;;;; kept here as I have no better place to put it. It probably will never
;;;;; be needed again.
;;;in Dr Scheme we must (require (lib "defmacro.ss"));; in order to define-macro
;;;;;; This was the origonal amb a macro where I really wanted amblist
; (define-macro amb
    ; (lambda alts...
        ; `(let ((+prev-amb-fail amb-fail))
            ; (call/cc
                ; (lambda (+sk)
                    ; ,@(map (lambda (alt)
                        ; `(call/cc
                            ; (lambda (+fk)
                                ; (set! amb-fail
                                    ; (lambda ()
                                        ; (set! amb-fail +prev-amb-fail)
                                        ; (+fk 'fail)))
                                    ; (+sk ,alt))))
                                ; alts...)
                            ; (+prev-amb-fail))))))

;;kalotan.scm
;;The Kalotans are a tribe with a peculiar quirk. Their males always tell the truth.
;;Their females never make two consecutive true statements, or two consecutive untrue
;;statements.
;;An anthropologist (let's call him Worf) has begun to study them. Worf does
;;not yet know the Kalotan language. One day, he meets a Kalotan (heterosexual)
;;couple and their child Kibi. Worf asks Kibi: ?Are you a boy?? Kibi answers in
;;Kalotan, which of course Worf doesn?t understand.
;;Worf turns to the parents (who know English) for explanation. One of them
;;says: ?Kibi said: ?I am a boy.? ? The other adds: ?Kibi is a girl. Kibi lied.?
;;Solve for the sex of the parents and Kibi.
;;
(load "amb.scm")

(define Gender '(M F))

(let*
    (
        (P1 (amblist Gender))
        (P2 (amblist (select-from Gender P1)))
        (Kibi (amblist Gender))
        (Kibitruth (amb #f #t))
    )

    (if (eqv? Kibi 'M)
        (required Kibitruth))

    (if (eqv? P1 'M)
        (if (eqv? Kibi 'F)
            (required (not Kibitruth)))
        (required (eqv? (eqv? Kibi 'F) Kibitruth))
    )

    (if (eqv? P2 'M)
        (required (and (eqv? Kibi 'F) (not Kibitruth))))

    (display `(P1 is ,P1 P2 is ,P2 Kibi is ,Kibi)) (newline)
    (amb) ;;???? more solutions
)

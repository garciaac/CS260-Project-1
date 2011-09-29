;; returns cons cell whose car points to 'COOP or 'DEFECT and the 
;; cdr points to the history structure
;(defun decide-aanz (opID history last-op-decision opchange mychange)
  ; last-op-desc to ophist (ophist now in sync)
  ; opchange, mychange to probdata
  ; make decision
  ; current-decision to ophist (ophist out of sync)
  ; return decision and history
  ; TODO

;                            ophist ----------------------------------------------          probdata-----------------
; history of form '(('ophist ( ( 1 '( c d ..) '(d c ..)) (2 '(d c ..) '(c d ..)) ) ) ('data (( 80 90 ..) ( 50 60 ..)) ))

; update ophist data with last-op-decision 
(defun ophist-op-update-aanz (opid ophist last-op-decision)
  (if last-op-decision
    (let ((entry (assoc opid ophist))) ; entry should never be nil, since if last-op-decision is not nil, hisory was populated by our own decision
      (setf (second entry) (cons last-op-decision (second entry))))))

; assumes ophist in sync (when looking up decisions made last round)
; update probdata with opchange, mychange, (does nothing on the first round against an opponent, ie when an entry doesnt exist in ophist)
(defun probdata-update-aanz (opid ophist probdata mychange opchange)
  (let ((entry (assoc opid ophist)))
    (if entry (let ((mychoice (first (second entry))) (opchoice (first (third entry))))
      ; mychoice, opchoice should not be nil, since if so, entry should also be nil
      (cond ((and (eq mychoice 'c) (eq opchoice 'c)) (setf (first probdata) (sort (cons opchange (cons mychange (first probdata))) #'<)))
            ((and (eq mychoice 'c) (eq opchoice 'd)) (setf (second probdata) (sort (cons mychange (second probdata)) #'<)))
            ((and (eq mychoice 'd) (eq opchoice 'c)) (setf (second probdata) (sort (cons opchange (second probdata)) #'<)))
            ((and (eq mychoice 'd) (eq opchoice 'd)) (setf (third probdata) (sort (cons opchange (cons mychange (third probdata))) #'<))))))))

; returns decision. assumes history is up to date
(defun make-decision (opid ophist prob-data) 
  (make-choice-aanz (look-up-aanz prob-data) (second (assoc opid ophist))))


; update ophist with my decision, creating an entry if one doesnt exist 
(defun ophist-my-update-aanz (opid ophist my-decision)
  (let ((entry (assoc opid ophist))) 
    (if (not entry) (setf (rest ophist) (cons (list opid nil (list my-decision)) (rest ophist)))
                        ; a little hack to make the change a side effect, we cant just change ophist
                        ; and order doesnt matter so we just insert it after the first element
                    (setf (third entry) (cons my-decision (third entry))))))


;; history is opponent specific and returns c or d
;; history of form ('c 'd 'd ...) most recent first
(defun make-choice-aanz (func s-ophistory)
  (if (< (rand-aanz) (make-choice-prob-aanz func s-ophistory)) 'c 'd))

;; helper for make choice, (gives probablity of cooperating)
(defun make-choice-prob-aanz (func s-ophistory)
  (let ((c-prob (second func)) (func-op (cddr func)))
    (progn (dotimes (i (length func-op)) 
      (progn (if (eq (first s-ophistory) 'd) (setf c-prob (+ c-prob (first func-op))))
             (setf func-op (rest func-op))
             (setf s-ophistory (rest s-ophistory))))
           (- 1 c-prob))))

; returns the function precribed by the probability distribution estimation
(defun look-up-aanz (prob-data)
  (let* ((funclength 12) (outfunc (zeron-aanz funclength)))
    (progn
      (dotimes (cc 20)
        (let ((weightcc (prob-aanz prob-data 'c 'c (+ 80 cc))))
          (dotimes (cd 100)
            (let ((weightcd (prob-aanz prob-data 'c 'd cd)))
              (dotimes (dd 50)
                (let* ((weightcc (prob-aanz prob-data 'd 'd (+ 40 cc)))
                       (weight (* weightcc weightcd weightcc))
                       (func (single-look-up-aanz (+ 80 cc) cd (+ 40 dd)))
                       (outptr outfunc))
                  (dotimes (i funclength) 
                    (progn (setf (car outptr) (+ (car outptr) (* weight (if (car func) (car func) 0))))
                           (setf func (cdr func))
                           (setf outptr (cdr outptr))))))))))
      (let ((norm (apply #'+ outfunc)) (funcptr outfunc))
        (if (> norm 0) 
          (dotimes (i funclength)
            (progn (setf (car funcptr) (/ (car funcptr) norm))
                   (setf funcptr (cdr funcptr))))))
      outfunc)))


; looks up the function for each of the expected values cc cd dd
(defun single-look-up-aanz (cc cd dd) 
  (second (assoc (- 100 dd) (second (assoc (- 100 cd) (second (assoc (- 100 cc) data)))))))
 

;; returns a real number which is the value of the probability
;; distribution evaluated at x
;; assumes prob-data is sorted
(defun prob-aanz (prob-data mychoice opchoice x)
  (let ((d (cond ((and (eq mychoice 'c) (eq opchoice 'c)) (first prob-data))
                 ((and (eq mychoice 'c) (eq opchoice 'd)) (second prob-data))
                 ((and (eq mychoice 'd) (eq opchoice 'd)) (third prob-data))
                 (T nil))) 
        (bot (cond ((and (eq mychoice 'c) (eq opchoice 'c)) 80)
                 ((and (eq mychoice 'c) (eq opchoice 'd)) 0)
                 ((and (eq mychoice 'd) (eq opchoice 'd)) 40)
                 (T 100))) 
        (top (cond ((and (eq mychoice 'c) (eq opchoice 'c)) 100)
                 ((and (eq mychoice 'c) (eq opchoice 'd)) 100)
                 ((and (eq mychoice 'd) (eq opchoice 'd)) 90)
                 (T 100)))
        last)
    ; sort here if data unsorted
    (or (dolist (a d)
      (let ((ap (if (= a top) (- a .5) a))) ; a hack to prevent divide by 0 errors when top = a = x
        (if (> ap x) (return (/ (if last 1.0 .5) (* (length d) (- ap (or last bot))))) (setf last ap))))
        (if last (/ .5 (* (length d) (- top last))) (/ 1.0 (- top bot))))))


; returns random number between 0 and 1
(defun rand-aanz () 
  (/ (random 4294967296) 4294967296.0))

; creates a list of n 0's
(defun zeron-aanz (n) 
  (if (= n 0) nil (cons 0 (zeron-aanz (- n 1)))))

;(load 'masterLisp.txt)
